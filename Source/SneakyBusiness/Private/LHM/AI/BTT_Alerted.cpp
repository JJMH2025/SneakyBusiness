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
	if (!Enemy)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 1. 회전 체크: 회전 중이면 이동 안함
	if(Enemy->IsRotating()) return;

	FVector EnemyLoc = Enemy->GetActorLocation();
	FVector TargetLoc = Enemy->GetAlertLocation();

	// 2. 방향 맞추기
	// 에너미의 방향에 따라 경보 위치로 이동하기 전 회전할지 여부
	// 에너미가 왼쪽을 바라보는지 판별
	bool bEnemyFacingLeft = Enemy->GetActorForwardVector().X < 0;
	// 경보 위치가 에너미의 왼쪽에 있는지 판별
	bool bIsAlertLeftOfEnemy = TargetLoc.X < EnemyLoc.X;
	// 에너미가 바라보는 방향에 경보 위치가 없다면 회전해야 함
	bool bShouldTurn = bIsAlertLeftOfEnemy != bEnemyFacingLeft;

	if (bShouldTurn)
	{
		float RotationAmount = Enemy->IsMovingForward() ? -180.0f : 180.0f;
		Enemy->SetTargetRotation(Enemy->GetActorRotation() + FRotator(0, RotationAmount, 0));
		Enemy->StartRotating();
	}

	// 3. 이동
	FVector Dir = FVector(TargetLoc.X - EnemyLoc.X, 0, 0).GetSafeNormal();
	Enemy->AddMovementInput(Dir, 0.5);

	// 4. 거리 체크
	// 목적지와 에너미의 거리 200 미만일 시 Patrol로 상태 전환
	if (FVector::Dist(TargetLoc, EnemyLoc) <= 200)
	{
		Enemy->SetEnemyAIState(EEnemyAIState::Patrol);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
	// 에너미 현재 상태가 Alerted이 아니면 해당 태스크 종료
	/*if (Enemy->GetEnemyAIState() != EEnemyAIState::Alerted)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}*/
}
