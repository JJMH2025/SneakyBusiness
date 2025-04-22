// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_Lever.h"

#include "MH/MH_EnemyAlertComp.h"

// Sets default values
AMH_Lever::AMH_Lever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyAlertComp = CreateDefaultSubobject<UMH_EnemyAlertComp>(TEXT("EnemyAlertComp"));

}

// Called when the game starts or when spawned
void AMH_Lever::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMH_Lever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMH_Lever::ActivateLever()
{
	bIsLeverActive = true;
	LeverAnim();
	EnemyAlertComp->AlertNearbyEnemies(GetActorLocation(), AlertRadius);
}

