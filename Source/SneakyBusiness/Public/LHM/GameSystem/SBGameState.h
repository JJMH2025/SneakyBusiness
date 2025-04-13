// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SBGameState.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API ASBGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	// 아이템 액터에서 획득 시 호출
	void CollectItem() { CollectedItemCount++; }

	// 에너미가 기절시킬 때 마다 호출
	void KilledEnemy();


	UPROPERTY(BlueprintReadOnly)
	float ElapsedTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeLimit = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentScore = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 CollectedItemCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredItemCount = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bRechedGoal = false;

	// 훔친 아이템 리스트 (런타임용)
	//UPROPERTY()
	//TArray<AActor*> CollectedTargetActors;

	// 세이브 시 사용할 인덱스 리스트 (직렬화 가능)
	UPROPERTY()
	TArray<int32> CollectedTargetIndices;
};
