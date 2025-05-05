// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_Attack.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LHM/AI/EnemyAIController.h"
#include "MH/MH_ShootComp.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attck");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Enemy || !BB)
	{
		return EBTNodeResult::Failed;
	}

	// 플레이어 감지 못하면 Patrol 전환
	if (!BB->GetValueAsObject("Player"))
	{
		Enemy->SetEnemyAIState(EEnemyAIState::Patrol);
		return EBTNodeResult::Succeeded;
	}

	// 총알 발사
	FVector FireLocation = Enemy->GetActorLocation();
	FRotator FireRotation = Enemy->GetActorRotation();
	Enemy->GetShootComp().Shooting(FireLocation, FireRotation);

	// 에너미 현재 상태가 Attack이 아니면 해당 태스크 종료
	if (Enemy->GetEnemyAIState() != EEnemyAIState::Attack)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}
