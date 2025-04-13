// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTD_CurrentState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CurrentState::UBTD_CurrentState()
{
    NodeName = "Is Current State";
}

bool UBTD_CurrentState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if(!BBComp) return false;

	FName StateName = BBComp->GetValueAsName(GetSelectedBlackboardKey());
	FString EnumStr = StaticEnum<EEnemyAIState>()->GetNameStringByValue((int64)DesiredState);
	FName DesiredName(*EnumStr);

	return StateName == DesiredName;
}