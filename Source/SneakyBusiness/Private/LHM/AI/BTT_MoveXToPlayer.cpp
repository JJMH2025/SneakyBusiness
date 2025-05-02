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
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_MoveXToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTT_MoveXToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Enemy || !Player)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector PlayerLoc = Player->GetActorLocation();
	FVector EnemyLoc = Enemy->GetActorLocation();

	// 플레이어와 에너미가 다른 공간이라면
	if (Player->bIsPlayerLoc != Enemy->IsASpace())
	{
		// 먼저 X축 정렬을 시도
		FVector Dir = FVector((PlayerLoc.X - EnemyLoc.X), 0, 0).GetSafeNormal();
		Enemy->AddMovementInput(Dir, 0.5f);

		// 플레이어와 에너미의 간격이 100 이하면 공간 이동 -> 해당 태스크 종료
		if (FMath::Abs(PlayerLoc.X - EnemyLoc.X) < 100.0f)
		{
			AEnemyAIController* AIController = Cast<AEnemyAIController>(Enemy->GetController());
			if (!AIController) return;

			AIController->SetBlackboardBoolValue("bIsPlayerInOtherSpace", true);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	else
	{
		// 에너미가 회전 중이거나 플레이어의 상태가 Frozen 또는 Dead라면 Patorl로 상태 전환 -> 해당 태스크 종료
		if (Enemy->IsRotating() || Enemy->IsPlayerStateToFrozenOrDead())
		{
			Enemy->SetEnemyAIState(EEnemyAIState::Patrol);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// 플레이어 쫓다가 장애물 있으면 뒤돌아서 Patorl로 상태 전환 -> 해당 태스크 종료
		if (Enemy->IsObstacleAhead(Enemy->GetActorForwardVector(), 130.0f))
		{
			UE_LOG(LogTemp, Log, TEXT("In Chase 장애물 감지"));
			float RotationAmount = Enemy->IsMovingForward() ? -180.0f : 180.0f;
			Enemy->SetTargetRotation(Enemy->GetActorRotation() + FRotator(0, RotationAmount, 0));
			Enemy->StartRotating();

			Enemy->SetEnemyAIState(EEnemyAIState::Patrol);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// 장애물이 없으면
		else
		{
			// 플레이어와의 거리 체크 (300 이하이면 Attack 상태로 전환) -> 해당 태스크 종료
			if (FVector::Dist(EnemyLoc, PlayerLoc) <= 300.0f)
			{
				Enemy->SetEnemyAIState(EEnemyAIState::Attack);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}

			// 플레이어와 거리가 300 이상이라면 플레이어 방향으로 이동
			else
			{
				FVector Dir = FVector((PlayerLoc.X - EnemyLoc.X), 0, 0).GetSafeNormal();
				Enemy->AddMovementInput(Dir, 0.5f);
			}
		}
	}

	// 에너미 현재 상태가 Chase가 아니면 해당 태스크 종료
	if (Enemy->GetEnemyAIState() != EEnemyAIState::Chase)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
