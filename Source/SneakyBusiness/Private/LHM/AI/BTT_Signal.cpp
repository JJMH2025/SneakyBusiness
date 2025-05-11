// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_Signal.h"
#include "LHM/Enemy/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LHM/AI/EnemyAIController.h"

UBTT_Signal::UBTT_Signal()
{
	NodeName = TEXT("Signal");
}

EBTNodeResult::Type UBTT_Signal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		AEnemyAIController* AIController = Cast<AEnemyAIController>(Enemy->GetController());
		if (AIController && Enemy->BT)
		{
			Enemy->SetEnemyAIState(EEnemyAIState::Patrol);
			AIController->RunBehaviorTree(Enemy->BT);
		}
		return EBTNodeResult::Succeeded;
	}

	// 신호 실행 (타입별로 override됨)
	Enemy->Signal();

	//// 에너미 현재 상태가 Signal이 아니면 해당 태스크 종료
	//if (Enemy->GetEnemyAIState() != EEnemyAIState::Signal)
	//{
	//	return EBTNodeResult::Succeeded;
	//}

	return EBTNodeResult::Succeeded;
}
