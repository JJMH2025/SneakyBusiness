// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/GameSystem/SBGameMode.h"
#include "LHM/GameSystem/SBGameInstance.h"
#include "LHM/GameSystem/SBGameState.h"
#include <MH/Player_Nick.h>
#include <Kismet/GameplayStatics.h>
#include "LHM/GameSystem/SBSaveGame.h"

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
	GI->CurrentStageIndex += 1;

	// 게임 데이터 저장
	USBSaveGame* Save = Cast<USBSaveGame>(UGameplayStatics::CreateSaveGameObject(USBSaveGame::StaticClass()));
	Save->StageScoreMap.Add(CurrentStageName, TotalScore);
	Save->StageTimeMap.Add(CurrentStageName, TimeLeft);
	Save->ClearedStages.Add(CurrentStageName);
	UGameplayStatics::SaveGameToSlot(Save, TEXT("PlayerSaveSlot"), 0);

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
