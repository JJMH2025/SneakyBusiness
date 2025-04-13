// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SBSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API USBSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// 스테이지 별 점수, 시간
	UPROPERTY()
	TMap<FName, int32> StageScoreMap;
	UPROPERTY()
	TMap<FName, float> StageTimeMap;

	// 각 스테이지 클리어 여부
	UPROPERTY()
	TSet<FName> ClearedStages;

	// 마지막 위치
	UPROPERTY()
	FVector LastPlayerLocation;
};
