// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/GameSystem/SBGameMode.h"
#include "LHM/GameSystem/SBGameInstance.h"
#include "LHM/GameSystem/SBGameState.h"
#include <MH/Player_Nick.h>
#include <Kismet/GameplayStatics.h>
#include "LHM/GameSystem/SBSaveGame.h"
#include <MH/MH_TargetItem.h>

void ASBGameMode::BeginPlay()
{
	Super::BeginPlay();

	ASBGameState* GS = GetGameState<ASBGameState>();
	USBGameInstance* GI = GetGameInstance<USBGameInstance>();
	if (!GS || !GI) return;
	
	switch (GI->CurrentStageIndex)
	{
		case 1:
			CurrentStageName = "Stage1";
			GS->RequiredItemCount = 4;
			GS->TimeLimit = 180;
			break;

		case 2:
			CurrentStageName = "Stage2";
			GS->RequiredItemCount = 6;
			GS->TimeLimit = 150;
			break;

		case 3:
			CurrentStageName = "Stage3";
			GS->RequiredItemCount = 8;
			GS->TimeLimit = 120;
			break;

		default :
			break;
	}
}

void ASBGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckClearConditions();
}

void ASBGameMode::OnStageClear()
{
	ASBGameState* GS = GetGameState<ASBGameState>();
	USBGameInstance* GI = GetGameInstance<USBGameInstance>();
	if (!GI || !GS) return;

	// 스코어 산정
	int32 CurrentScore = GS->CurrentScore;					// 현재 스코어
	float TimeLeft = GS->TimeLimit - GS->ElapsedTime * 100;	// 남은 시간
	int32 TotalScore = CurrentScore + TimeLeft;				// 최종 스코어
	
	// 게임 인스턴스에 최종 스코어 저장
	GI->SetTotalScore(TotalScore);

	// 게임 데이터 저장
	USBSaveGame* Save = Cast<USBSaveGame>(UGameplayStatics::CreateSaveGameObject(USBSaveGame::StaticClass()));
	APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(!Save || !Player) return;

	FStageSaveData& Data = Save->StageDataMap.FindOrAdd(GI->CurrentStageIndex);
	Data.StageIndex = GI->CurrentStageIndex;
	Data.Score = TotalScore;
	Data.TimeLeft = TimeLeft;
	Data.bCleared = true;
	Data.LastPlayerLocation = Player->GetActorLocation();
	Data.StolenItems = GS->StolenItems;

	Save->StageDataMap.Add(GI->CurrentStageIndex, Data);

	UGameplayStatics::SaveGameToSlot(Save, TEXT("PlayerSaveSlot"), 0);

	GI->CurrentStageIndex += 1;

	// Steam 업적 호출
	// EX: ISBSteamAPI::UnlockAchievement("ACH_Stage1_CLEAR");

	// 랭크 UI 호출 또는 Level 전환
	//CreateWidget<UUserWidget_RankResult>(...) → AddToViewport()
	//FName NextStageName = FName(TEXT("Stage%d"), GI->CurrentStageIndex);
	//UGameplayStatics::OpenLevel(this, NextStageName);
}

void ASBGameMode::OnStageFailed()
{
	UE_LOG(LogTemp, Log, TEXT("Stage Failed."));
	// RETRY UI 표시 또는 메인 메뉴 전환
}

int32 ASBGameMode::GetRequiredItemCount() const
{
	ASBGameState* GS = GetGameState<ASBGameState>();
	return GS->RequiredItemCount;
}

void ASBGameMode::OnItemStolen(int32 StageIndex, int32 TargetIndex)
{
	ASBGameState* GS = GetGameState<ASBGameState>();
	if (!GS) return;

	// 이미 존재하는 도둑질인지 확인
	bool bAlreadyStolen = GS->StolenItems.ContainsByPredicate([&](const FStolenItemInfo& Item){
		return Item.StageIndex == StageIndex && Item.ItemIndex == TargetIndex;
	});

	if (!bAlreadyStolen)
	{
		GS->CollectItem(); // 카운트 증가

		FStolenItemInfo Info;
		Info.StageIndex = StageIndex;
		Info.ItemIndex = TargetIndex;
		GS->StolenItems.Add(Info);
	}
}

void ASBGameMode::DropItemsOnDeath(FVector DeathLocation)
{
	ASBGameState* GS = GetGameState<ASBGameState>();
	if(!GS || AllTargetItemBPs.Num() == 0 || GS->StolenItems.Num() == 0) return;

	const float SpreadRadius = 100;

	for (const FStolenItemInfo& Info : GS->StolenItems)
	{
		int32 SpawnIndex = Info.StageIndex * GetRequiredItemCount() + Info.ItemIndex;

		if(!AllTargetItemBPs.IsValidIndex(SpawnIndex)) continue;

		FVector SpawnLocation = DeathLocation + FVector(
			FMath::RandRange(-SpreadRadius, SpreadRadius), 0, 30
		);

		AActor* Spawned = GetWorld()->SpawnActor<AActor>(
			AllTargetItemBPs[SpawnIndex],
			SpawnLocation,
			FRotator::ZeroRotator);

		if (AMH_TargetItem* Item = Cast<AMH_TargetItem>(Spawned))
		{
			Item->StageIndex = Info.StageIndex;
			Item->ItemIndex = Info.ItemIndex;
		}
	}

	GS->StolenItems.Empty();  // 리셋
}

void ASBGameMode::CheckClearConditions()
{
	ASBGameState* GS = GetGameState<ASBGameState>();
	APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!GS || !Player) return;

	// 목표 아이템을 모두 획득하고 목표 지점에 도착했을 때 Clear
	if (GS->bRechedGoal && GS->CollectedItemCount == GS->RequiredItemCount)
	{
		OnStageClear();
	}
	
	// 제한 시간 초과했거나 플레이어가 사망 상태일 때 Fail
	if (GS->ElapsedTime >= GS->TimeLimit || Player->CurrentPlayerState == EPlayerState::Dead)
	{
		OnStageFailed();
	}
}
