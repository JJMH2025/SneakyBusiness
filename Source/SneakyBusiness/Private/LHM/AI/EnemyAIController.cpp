// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/EnemyAIController.h"
#include "LHM/Enemy/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemy* Enemy = Cast<AEnemy>(GetPawn());

	if (Enemy && UseBlackboard(Enemy->BBD, BBComp))
	{
		RunBehaviorTree(Enemy->BT);

		Blackboard->SetValueAsObject("SelfActor", Enemy);

		// 런타임에 Combat Subtree 설정
		Blackboard->SetValueAsObject("CombatSubtree", Enemy->CombatSubtree);
	}
}

void AEnemyAIController::SetBlackboardBoolValue(FName KeyName, bool bValue)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(KeyName, bValue);
	}
}
