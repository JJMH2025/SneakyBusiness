// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTD_CurrentState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CurrentState::UBTD_CurrentState()
{
	NodeName = TEXT("Is Current State");
}

bool UBTD_CurrentState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if(!BBComp) return false;

	FName StateName = BBComp->GetValueAsName(GetSelectedBlackboardKey());
	FString EnumStr = StaticEnum<EEnemyAIState>()->GetNameStringByValue((int64)DesiredState);
	EnumStr = EnumStr.RightChop(FString("EEnemyAIState::").Len());

	FName DesiredName(*EnumStr);
	return StateName == DesiredName;
}
