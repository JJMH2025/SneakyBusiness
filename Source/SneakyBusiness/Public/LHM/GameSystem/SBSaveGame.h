// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SBGameState.h"
#include "SBSaveGame.generated.h"

/**
 * 
 */

 USTRUCT(BlueprintType)
struct FStageSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 StageIndex;

    UPROPERTY()
    int32 Score;

    UPROPERTY()
    float TimeLeft;

    UPROPERTY()
    bool bCleared;

    UPROPERTY()
    FVector LastPlayerLocation;

    UPROPERTY()
    TArray<FStolenItemInfo> StolenItems;
};

UCLASS()
class SNEAKYBUSINESS_API USBSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
    TMap<int32, FStageSaveData> StageDataMap; // StageIndex별 데이터 저장
};
