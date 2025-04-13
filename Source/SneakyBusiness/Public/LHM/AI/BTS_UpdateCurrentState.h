// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateCurrentState.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UBTS_UpdateCurrentState : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_UpdateCurrentState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSectonds) override;
};
