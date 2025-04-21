// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_EnemyAlertComp.h"

#include "Kismet/GameplayStatics.h"
#include "LHM/Enemy.h"

// Sets default values for this component's properties
UMH_EnemyAlertComp::UMH_EnemyAlertComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMH_EnemyAlertComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMH_EnemyAlertComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMH_EnemyAlertComp::AlertNearbyEnemies(FVector SourceLocation, float Radius)
{
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);

	for (AActor* EnemyActor : FoundEnemies)
	{
		float Dist = FVector::Dist(EnemyActor->GetActorLocation(), SourceLocation);
		if (Dist <= Radius)
		{
			AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
			if (Enemy)
			{
				//Enemy->OnAlerted(SourceLocation); // 이 함수는 에너미에 구현돼 있어야겠지
			}
		}
	}
}