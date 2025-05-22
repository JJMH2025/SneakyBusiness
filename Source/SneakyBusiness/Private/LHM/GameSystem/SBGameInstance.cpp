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

    // 스코어 작성 구조체 세팅
    FOnlineLeaderboardWrite WriteObject;
    //WriteObject.LeaderboardNames.Add(FName(("Stage%d_Leaderboard"), CurrentStageIndex));
    WriteObject.LeaderboardNames.Add(FString::Printf(TEXT("Stage%d_Leaderboard"), CurrentStageIndex));
    //WriteObject.RatedStat = FName("Score");
    WriteObject.RatedStat = FString::Printf(TEXT("Score"));
    WriteObject.DisplayFormat = ELeaderboardFormat::Number;
    WriteObject.SortMethod = ELeaderboardSort::Descending;
    WriteObject.UpdateMethod = ELeaderboardUpdateMethod::KeepBest;
    
    // 핵심 : 점수 설정
    //WriteObject.SetIntStat(FName("Score"), Score);
    WriteObject.SetIntStat(FString::Printf(TEXT("Score")), Score);

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
    //LeaderboardRead->LeaderboardName = FName("Stage1_Leaderboard");
    LeaderboardRead->LeaderboardName = FString::Printf(TEXT("Stage%d_Leaderboard"), CurrentStageIndex);

    // 델리게이트 바인딩
    FOnLeaderboardReadCompleteDelegate Delegate = FOnLeaderboardReadCompleteDelegate::CreateUObject(this, &USBGameInstance::OnLeaderboardReadComplete);
    ReadCompleteHandle = Leaderboards->AddOnLeaderboardReadCompleteDelegate_Handle(Delegate);

    // 랭킹 1~10위 조회 요청
    Leaderboards->ReadLeaderboardsAroundRank(1, 10, LeaderboardRead);
}

void USBGameInstance::OnLeaderboardReadComplete(bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        Subsystem->GetLeaderboardsInterface()->ClearOnLeaderboardReadCompleteDelegate_Handle(ReadCompleteHandle);
    }

    if (!bWasSuccessful || !LeaderboardRead.ToSharedPtr().IsValid()) return;

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
        const FVariantData* ScoreData = Row.Columns.Find(FString::Printf(TEXT("Score")));
        if (ScoreData)
        {
            ScoreData->GetValue(Entry.Score);
        }

        TopRanks.Add(Entry);

        // 내 랭크 저장
        IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
        IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
        if (Identity)
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
    }

}
