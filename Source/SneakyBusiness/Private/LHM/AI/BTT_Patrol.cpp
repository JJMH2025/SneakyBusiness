// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_Patrol.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_Patrol::UBTT_Patrol()
{
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if(!Enemy) return EBTNodeResult::Failed;

	Enemy->Patrol();
	return EBTNodeResult::Succeeded;
}
