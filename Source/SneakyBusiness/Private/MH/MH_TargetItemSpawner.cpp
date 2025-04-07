// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_TargetItemSpawner.h"

#include "MH/MH_TargetItem.h"

// Sets default values
AMH_TargetItemSpawner::AMH_TargetItemSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMH_TargetItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	int32 Index = StageIndex * 3 + TargetIndex;
	if (!SpawnedItems.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid item index: %d"), Index);
		return;
	}

	TSubclassOf<AActor> SpawnActor = SpawnedItems[Index];
	if (SpawnActor)
	{
		FActorSpawnParameters Params;
		AMH_TargetItem* SpawnedItem = GetWorld()->SpawnActor<
			AMH_TargetItem>(SpawnActor, GetActorLocation(), GetActorRotation(), Params);

		if (SpawnedItem)
		{
			//오버랩 체크용
			SpawnedItem->Tags.Add("TargetItem");
			SpawnedItem->StageIndex = StageIndex;
			SpawnedItem->ItemIndex = TargetIndex;
		}
	}
}

// Called every frame
void AMH_TargetItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
