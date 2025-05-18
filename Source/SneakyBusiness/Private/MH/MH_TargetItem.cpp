// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_TargetItem.h"

// Sets default values
AMH_TargetItem::AMH_TargetItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

}

// Called when the game starts or when spawned
void AMH_TargetItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMH_TargetItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

