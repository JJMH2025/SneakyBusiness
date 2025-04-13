// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_WakeUp.h"
#include <LHM/Enemy.h>
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_WakeUp::UBTT_WakeUp()
{
	NodeName = TEXT("WakeUp");
}

EBTNodeResult::Type UBTT_WakeUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	Enemy->WakeUp();
	return EBTNodeResult::Succeeded;
}
