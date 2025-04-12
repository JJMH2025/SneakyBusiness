// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveYToOtherSpace.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UBTT_MoveYToOtherSpace : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_MoveYToOtherSpace();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
