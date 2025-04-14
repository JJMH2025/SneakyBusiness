// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_Chase.h"
#include "LHM/Enemy.h"
#include "MH/Player_Nick.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_Chase::UBTT_Chase()
{
	NodeName = TEXT("Chase");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTT_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return;

	Enemy->Chase();

	if (Enemy->GetEnemyAIState() != EEnemyAIState::Chase)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
