// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Signal.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UBTT_Signal : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Signal();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
