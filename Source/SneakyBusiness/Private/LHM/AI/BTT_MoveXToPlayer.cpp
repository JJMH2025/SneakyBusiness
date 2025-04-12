// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_MoveXToPlayer.h"
#include "LHM/Enemy.h"
#include "MH/Player_Nick.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LHM/AI/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

UBTT_MoveXToPlayer::UBTT_MoveXToPlayer()
{
	NodeName = TEXT("Move X To Player");
}

EBTNodeResult::Type UBTT_MoveXToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (!Enemy) return EBTNodeResult::Failed;

	Enemy->AlignXToPlayer();
	return EBTNodeResult::Succeeded;
}
