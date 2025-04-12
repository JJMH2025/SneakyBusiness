// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_MoveYToOtherSpace.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_MoveYToOtherSpace::UBTT_MoveYToOtherSpace()
{
	NodeName = TEXT("Move Y To Other Space");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_MoveYToOtherSpace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTT_MoveYToOtherSpace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return;

	Enemy->PrepareMoveToOtherSpace();

	if (Enemy->CurrentState != EEnemyAIState::MovingToOtherSpace)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
