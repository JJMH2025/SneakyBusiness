// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MH/MH_TargetItem.h"
#include "SBGameMode.generated.h"

/**
 * 
 */

UCLASS()
class SNEAKYBUSINESS_API ASBGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASBGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void OnStageClear();
	void OnStageFailed();

	int32 GetRequiredItemCount() const;

	void OnItemStolen(int32 StageIndex, int32 TargetIndex, AMH_TargetItem* TargetItem);
	void DropItemsOnDeath(FVector DeathLocation);
	
    // 모든 아이템 목록
    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<class AMH_TargetItem>> AllTargetItemBPs;

private:
	void CheckClearConditions();

	// 스테이지 이름: 랭킹 저장에 사용
	FName CurrentStageName = "";

};
