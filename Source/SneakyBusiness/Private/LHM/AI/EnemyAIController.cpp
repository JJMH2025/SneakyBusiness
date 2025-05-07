// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/EnemyAIController.h"
#include "LHM/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemy* Enemy = Cast<AEnemy>(GetPawn());
	if (Enemy && UseBlackboard(Enemy->BBD, BBComp))
	{
		RunBehaviorTree(Enemy->BT);

		// 런타임에 서브트리 설정
		//Blackboard->SetValueAsObject(TEXT("SelectorSubtree"), Enemy->CombatSubtree);
	}
}

void AEnemyAIController::SetBlackboardBoolValue(FName KeyName, bool bValue)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(KeyName, bValue);
	}
}
