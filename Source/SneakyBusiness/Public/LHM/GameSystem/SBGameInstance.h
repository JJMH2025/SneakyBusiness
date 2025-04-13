// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API USBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BluePrintReadWrite)
	int32 ToatlScore = 0;

	UPROPERTY(BluePrintReadWrite)
	int32 CurrentStageIndex = 1;

	void SetTotalScore(int32 ScoreToAdd) { ToatlScore += ScoreToAdd; }
};
