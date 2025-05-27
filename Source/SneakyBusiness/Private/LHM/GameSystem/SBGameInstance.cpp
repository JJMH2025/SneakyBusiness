// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/GameSystem/SBGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineStats.h"
#include "LHM/UI/RankResultWidget.h"
#include "Kismet/GameplayStatics.h"

void USBGameInstance::UploadScoreToLeaderboard(int32 Score)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (!Subsystem) return;

    IOnlineLeaderboardsPtr Leaderboards = Subsystem->GetLeaderboardsInterface();
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
    if (!Leaderboards.IsValid() || !Identity.IsValid()) return;

    TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
    if (!UserId.IsValid()) return;

    FString LeaderboardNameStr = FString::Printf(TEXT("Stage%d_Leaderboard"), CurrentStageIndex);
    FName LeaderboardName = FName(*LeaderboardNameStr);

    // 스코어 작성 구조체 세팅
    FOnlineLeaderboardWrite WriteObject;
    //WriteObject.LeaderboardNames.Add(FName(("Stage%d_Leaderboard"), CurrentStageIndex));
    //FString NameStr = FString::Printf(TEXT("Stage%d_Leaderboard"), CurrentStageIndex);
    //WriteObject.LeaderboardNames.Add(FName(*NameStr));
    WriteObject.LeaderboardNames.Empty(); // 혹시 이전 값 남아있으면 제거
    WriteObject.LeaderboardNames.Add(LeaderboardName);
    WriteObject.RatedStat = FName("Score");
    WriteObject.DisplayFormat = ELeaderboardFormat::Number;
    WriteObject.SortMethod = ELeaderboardSort::Descending;
    WriteObject.UpdateMethod = ELeaderboardUpdateMethod::KeepBest;
    
    // 핵심 : 점수 설정
    WriteObject.SetIntStat(FName("Score"), Score);

    // 업로드
    Leaderboards->WriteLeaderboards(TEXT("GameSession"), *UserId, WriteObject);
    Leaderboards->FlushLeaderboards(TEXT("GameSession"));
}

void USBGameInstance::ReadTopRankers()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    IOnlineLeaderboardsPtr Leaderboards = Subsystem->GetLeaderboardsInterface();
    if (!Leaderboards.IsValid()) return;

    // 읽기용 구조체 생성
    LeaderboardRead = MakeShared<FOnlineLeaderboardRead>();

    FString NameStr = FString::Printf(TEXT("Stage%d_Leaderboard"), CurrentStageIndex);
    LeaderboardRead->LeaderboardName = FName(*NameStr);

    UE_LOG(LogTemp, Log, TEXT("[Rank] GameInstance address: %p"), this);

    // 델리게이트 생성 및 바인딩
    FOnLeaderboardReadCompleteDelegate Delegate = FOnLeaderboardReadCompleteDelegate::CreateUObject(this, &USBGameInstance::OnLeaderboardReadComplete);
    ReadCompleteHandle = Leaderboards->AddOnLeaderboardReadCompleteDelegate_Handle(Delegate);

    UE_LOG(LogTemp, Log, TEXT("[Rank] Registered delegate. Handle is valid: %s"), ReadCompleteHandle.IsValid() ? TEXT("YES") : TEXT("NO"));

    // 비동기 요청
    TSharedRef<FOnlineLeaderboardRead> ReadRef = LeaderboardRead.ToSharedRef();
    bool bStarted = Leaderboards->ReadLeaderboardsAroundRank(1, 10, ReadRef);
    
    UE_LOG(LogTemp, Log, TEXT("[Rank] Started = %s"), bStarted ? TEXT("YES") : TEXT("NO"));

    if (!bStarted)
    {
        UE_LOG(LogTemp, Error, TEXT("[Rank] Failed to start ReadLeaderboardsAroundRank."));
        Leaderboards->ClearOnLeaderboardReadCompleteDelegate_Handle(ReadCompleteHandle);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[Rank] Started ReadLeaderboardsAroundRank successfully."));
    }

    // 랭킹 1~10위 조회 요청
    //Leaderboards->ReadLeaderboardsAroundRank(1, 10, ReadRef);
}

void USBGameInstance::OnLeaderboardReadComplete(bool bWasSuccessful)
{

    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        /*Subsystem->GetLeaderboardsInterface()->ClearOnLeaderboardReadCompleteDelegate_Handle(ReadCompleteHandle);*/
        IOnlineLeaderboardsPtr Leaderboards = Subsystem->GetLeaderboardsInterface();
        if (Leaderboards.IsValid())
        {
            Leaderboards->ClearOnLeaderboardReadCompleteDelegate_Handle(ReadCompleteHandle);
        }
    }

    if (!bWasSuccessful || !LeaderboardRead.IsValid()) /*return;*/
    {
        UE_LOG(LogTemp, Error, TEXT("[Rank] Leaderboard read failed or data invalid."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[Rank] Leaderboard read completed successfully."));

    // 최종 데이터
    TArray<FRankEntry> TopRanks;
    int32 MyScore = 0;
    int32 MyRank = 0;

    for (const auto& Row : LeaderboardRead->Rows)
    {
        FRankEntry Entry;
        Entry.PlayerName = Row.NickName;
        Entry.Rank = Row.Rank;

        // 점수 추출 
        const FVariantData* ScoreData = Row.Columns.Find(FString(TEXT("Score")));
        if (ScoreData)
        {
            ScoreData->GetValue(Entry.Score);
        }

        TopRanks.Add(Entry);

        // 내 랭크 저장
        //IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
        //IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
        IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            TSharedPtr<const FUniqueNetId> MyId = Identity->GetUniquePlayerId(0);
            if (Row.PlayerId == MyId)
            {
                MyRank = Entry.Rank;
                MyScore = Entry.Score;
            }
        }
        UE_LOG(LogTemp, Log, TEXT("Rank %d: %s - %d"), Entry.Rank, *Entry.PlayerName, Entry.Score);
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC && RankResultWidgetClass)
    {
        URankResultWidget* RankUI = CreateWidget<URankResultWidget>(PC, RankResultWidgetClass);
        if (RankUI)
        {
            RankUI->InitRankResult(CurrentStageIndex, MyScore, MyRank, TopRanks);
            RankUI->AddToViewport();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Rank] Failed to create RankResultWidget."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[Rank] PlayerController or RankResultWidgetClass is null."));
    }
}
