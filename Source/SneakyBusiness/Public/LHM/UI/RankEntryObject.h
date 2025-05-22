// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RankEntryObject.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API URankEntryObject : public UObject
{
	GENERATED_BODY()
	
public:
	// Getter / Setter
	FString GetPlayerName() { return PlayerName; }
	int32 GetScore() { return Score; }
	int32 GetRank() { return Rank; }

	void SetPlayerName(FString _PlayerName) { PlayerName = _PlayerName; }
	void SetScore(int32 _Score) { Score = _Score; }
	void SetRank(int32 _Rank) { Rank = _Rank; }

private:
	FString PlayerName;
	int32 Score;
	int32 Rank;
};
