// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTS_UpdateCurrentState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LHM/AI/EnemyAIController.h"
#include "LHM/Enemy.h"

UBTS_UpdateCurrentState::UBTS_UpdateCurrentState()
{
	NodeName = TEXT("Update AI Current State");
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTS_UpdateCurrentState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSectonds)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Enemy || !BB) return;

	// CurrentState
	UEnum* EnumPtr = StaticEnum<EEnemyAIState>();
	FString EnumStr = EnumPtr->GetNameStringByValue((int64)Enemy->GetEnemyAIState());
	BB->SetValueAsName("CurrentState", FName(*EnumStr));
}
