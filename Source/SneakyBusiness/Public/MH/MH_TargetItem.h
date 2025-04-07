// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MH_TargetItem.generated.h"

UCLASS()
class SNEAKYBUSINESS_API AMH_TargetItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMH_TargetItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemIndex;

};
