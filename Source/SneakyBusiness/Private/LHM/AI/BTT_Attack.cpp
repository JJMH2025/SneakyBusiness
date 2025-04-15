// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_Attack.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attck");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return;

	Enemy->Attack();

	if (Enemy->GetEnemyAIState() != EEnemyAIState::Attack)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
