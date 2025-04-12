// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_Attack.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attck");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	Enemy->Attack();
	return EBTNodeResult::Succeeded;
}
