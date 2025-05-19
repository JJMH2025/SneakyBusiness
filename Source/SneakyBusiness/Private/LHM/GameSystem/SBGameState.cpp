// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/GameSystem/SBGameState.h"
#include "LHM/Enemy/Enemy.h"

ASBGameState::ASBGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASBGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
}

void ASBGameState::AddScoreForEnemy(class AEnemy* Enemy, bool bIsFinalStun)
{
	if (!Enemy) return;

	int32 ScoreToAdd = 0;
	EEnemyType Type = Enemy->GetEnemyType();

	// 에너미 타입에 따라 점수 합산
	switch (Type)
	{
		case EEnemyType::MeleeWhistle:
		case EEnemyType::ShooterFlare:
			ScoreToAdd = 100;
			break;

		case EEnemyType::ShooterShielded:
		case EEnemyType::MeleeShieldWhistle:
			ScoreToAdd = 150;
			break;

		case EEnemyType::Bomber:
			ScoreToAdd = bIsFinalStun ? 300 : 200;
			break;

		case EEnemyType::None:
			UE_LOG(LogTemp, Log, TEXT("EnemyType::None"));
			break;
		default:
			break;
	}

	CurrentScore += ScoreToAdd;
	UE_LOG(LogTemp, Log, TEXT("Score +%d from %s. Total: %d"), ScoreToAdd, *UEnum::GetValueAsString(Type), CurrentScore);
}
