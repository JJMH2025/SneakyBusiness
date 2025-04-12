// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "LHM/Enemy.h"
#include "BTD_CurrentState.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UBTD_CurrentState : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTD_CurrentState();
 
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	// 비교할 상태값
	UPROPERTY(EditAnyWhere, Category = "AI")
	EEnemyAIState DesiredState;
};
