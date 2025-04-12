// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_HitByDoor.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UBTT_HitByDoor : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_HitByDoor();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
