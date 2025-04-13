// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MH_LiftActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLiftArrivalDelegate);

UCLASS()
class SNEAKYBUSINESS_API AMH_LiftActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMH_LiftActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	//이동할 위치
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LiftSettings")
	FVector TargetLocation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LiftSettings")
	float LiftSpeed = 200.0f;

	UPROPERTY()
	bool bIsMoving = false;

	FVector StartLocation;

	float JourneyLength = 0.f;
	float MoveAlpha = 0.f;

	UFUNCTION()
	void ActivateLift();

	UFUNCTION()
	bool IsMoving()const;

	UPROPERTY(BlueprintAssignable)
	FLiftArrivalDelegate OnLiftArrived;
};

