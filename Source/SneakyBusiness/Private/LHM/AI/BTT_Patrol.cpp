// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_Patrol.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Enemy || !BB)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 1. 회전 체크: 회전 중이면 이동 안함
	if (Enemy->IsRotating()) return;

	// 2. 장애물 감지
	if (Enemy->IsObstacleAhead(Enemy->GetActorForwardVector(), 150.0f))
	{
		UE_LOG(LogTemp, Log, TEXT("In Patrol 장애물 감지"));

		// 방향 반전 + 회전
		float RotationAmount = Enemy->IsMovingForward() ? -180.0f : 180.0f;
		Enemy->SetTargetRotation(Enemy->GetActorRotation() + FRotator(0, RotationAmount, 0));
		Enemy->StartRotating();
	}

	// 3. 정면으로 이동
	Enemy->AddMovementInput(Enemy->GetActorForwardVector(), 0.2f);

	// 플레이어 감지하면 Chase 전환
	if (BB->GetValueAsObject("Player"))
	{
		Enemy->SetEnemyAIState(EEnemyAIState::Chase);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 에너미 현재 상태가 Patrol이 아니면 해당 태스크 종료
	if (Enemy->GetEnemyAIState() != EEnemyAIState::Patrol)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
