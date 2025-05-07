// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_TakeDamage.h"
#include <LHM/Enemy.h>
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_TakeDamage::UBTT_TakeDamage()
{
	NodeName = TEXT("Take Damage");
}

EBTNodeResult::Type UBTT_TakeDamage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	// 총에 맞는 애니메이션 재생
	UE_LOG(LogTemp, Log, TEXT("Play Montage: Enemy Take Damage"));

	return EBTNodeResult::Succeeded;
}
