// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MH_TargetItemSpawner.generated.h"

UCLASS()
class SNEAKYBUSINESS_API AMH_TargetItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMH_TargetItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category="Spawn Info")
	int32 StageIndex;

	UPROPERTY(EditAnywhere,Category="Spawn Info")
	int32 TargetIndex;

	UPROPERTY(EditAnywhere,Category="Spawn Info")
	TArray<TSubclassOf<AActor>> SpawnedItems;

	
	

};
