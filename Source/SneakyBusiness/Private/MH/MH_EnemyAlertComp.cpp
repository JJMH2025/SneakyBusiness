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
		FVector  EnemyLocation = EnemyActor->GetActorLocation();
		float XY_Dist = FVector::Dist2D(EnemyLocation, SourceLocation); // Z축 무시
		float Z_Diff = FMath::Abs(EnemyLocation.Z - SourceLocation.Z);  // Z 높이 차이
		if (XY_Dist <= Radius && Z_Diff < 200.f) // Z 차이 제한을 설정해줌
		{
			AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
			if (Enemy)
			{
				Enemy->ReactToTrapAlert(SourceLocation);
				GEngine->AddOnScreenDebugMessage(-2, 3.f, FColor::Red, "AlertNearbyEnemies11111");
			}
		}
	}
}