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
	}
}
