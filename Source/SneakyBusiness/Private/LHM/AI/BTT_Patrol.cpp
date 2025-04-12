// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_Patrol.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_Patrol::UBTT_Patrol()
{
	NodeName = TEXT("Patrol");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTT_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return;

	Enemy->Patrol();

	if (Enemy->CurrentState != EEnemyAIState::Patrol)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
