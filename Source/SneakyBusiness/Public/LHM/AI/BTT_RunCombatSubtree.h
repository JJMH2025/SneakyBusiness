// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RunCombatSubtree.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UBTT_RunCombatSubtree : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_RunCombatSubtree();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
