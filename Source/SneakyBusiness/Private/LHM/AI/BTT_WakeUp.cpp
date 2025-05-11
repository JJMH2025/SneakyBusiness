// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTT_WakeUp.h"
#include "LHM/Enemy/Enemy.h"
#include "LHM/AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_WakeUp::UBTT_WakeUp()
{
	NodeName = TEXT("Wake Up");
}

EBTNodeResult::Type UBTT_WakeUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	AEnemyAIController* Controller = Cast<AEnemyAIController>(Enemy->GetController());
	UBlackboardComponent* BB = Controller->GetBlackboardComponent();
	if (!Enemy || !Controller || !BB) return EBTNodeResult::Failed;

	// 일어나는 애니메이션 재생
	UE_LOG(LogTemp, Log, TEXT("Play Montage: Enemy Wake up"));

	// 블랙보드 키 초기화
	BB->SetValueAsBool(FName("bHitByDoor"), false);
	BB->SetValueAsBool(FName("bTakeDamage"), false);

	// Patrol 상태로 복귀
	Enemy->SetEnemyAIState(EEnemyAIState::Patrol);

	return EBTNodeResult::Succeeded;
}
