// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/GameSystem/SBGameState.h"

ASBGameState::ASBGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASBGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
}

void ASBGameState::KilledEnemy()
{
	// 매개변수로 에너미 타입을 받음
	// 에너미 타입에 따른 점수 합산해주기
	// Switch(EnemyType)

	CurrentScore++;
}
