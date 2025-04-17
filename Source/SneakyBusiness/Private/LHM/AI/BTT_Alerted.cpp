// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_Alerted.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_Alerted::UBTT_Alerted()
{
	NodeName = TEXT("Alerted");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Alerted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTT_Alerted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return;

	Enemy->Alerted();

	if (Enemy->GetEnemyAIState() != EEnemyAIState::Alerted)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
