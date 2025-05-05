// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_MoveYToOtherSpace.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LHM/AI/EnemyAIController.h"
#include "MH/Player_Nick.h"
#include "Kismet/GameplayStatics.h"

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
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Enemy || !BB)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 플레이어 감지 못하면 Patrol로 상태 전환 -> 해당 태스크 종료
	if (!BB->GetValueAsObject("Player"))
	{
		Enemy->SetEnemyAIState(EEnemyAIState::Patrol);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (!Enemy->IsMovingDepth())
	{
		FVector EnemyLoc = Enemy->GetActorLocation();

		float OffsetY = Enemy->IsASpace() ? -90.0f : 90.0f; // A → B : -90 / B → A : +90
		Enemy->SetMoveDepthLocation(EnemyLoc + FVector(0, OffsetY, 0));

		FVector DirectionToDepth;

		// 에너미가 왼쪽을 보고 있으면 오른쪽 방향 감지
		if (Enemy->GetActorForwardVector().X < 0)
		{
			DirectionToDepth = Enemy->GetActorRightVector();
		}
		// 에너미가 오른쪽을 보고 있으면 왼쪽 방향 감지
		else
		{
			DirectionToDepth = -Enemy->GetActorRightVector();
		}

		// B공간으로 이동할 때 장애물 있으면 다시 X축 정렬
		if (Enemy->IsObstacleAhead(DirectionToDepth, 90))
		{
			UE_LOG(LogTemp, Log, TEXT("In Prepare Move To Other Space 장애물 감지"));
			APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (!Player) return;

			FVector PlayerLoc = Player->GetActorLocation();

			// X축 정렬 시도
			FVector Dir = FVector((PlayerLoc.X - EnemyLoc.X), 0, 0).GetSafeNormal();
			Enemy->AddMovementInput(Dir, 0.5f);

			if (FMath::Abs(PlayerLoc.X - EnemyLoc.X) < 1.0f)
			{
				Enemy->StartMovingDepth();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}
		// 장애물 없으면 B공간으로 이동
		else
		{
			UE_LOG(LogTemp, Log, TEXT("In Prepare Move To Other Space 장애물 없음 바로 StartMovingDepth"));

			Enemy->StartMovingDepth();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	if (Enemy->GetEnemyAIState() != EEnemyAIState::Chase)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
