// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_LiftActor.h"

#include "Components/BoxComponent.h"
#include "MH/Player_Nick.h"

// Sets default values
AMH_LiftActor::AMH_LiftActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//BoxComp
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	

}

// Called when the game starts or when spawned
void AMH_LiftActor::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	JourneyLength = FVector::Dist(StartLocation, TargetLocation);
}

// Called every frame
void AMH_LiftActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		MoveAlpha += (LiftSpeed * DeltaTime) / JourneyLength;
		MoveAlpha = FMath::Clamp(MoveAlpha, 0, 1);

		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, MoveAlpha);
		SetActorLocation(NewLocation);

		if (MoveAlpha >= 1.f)
		{
			bIsMoving = false;
			// 리프트 도착 완료!
			OnLiftArrived.Broadcast();
		}
	}

}

void AMH_LiftActor::ActivateLift()
{
	if (!bIsMoving)
	{
		bIsMoving = true;
		MoveAlpha = 0.f;
	}
}

bool AMH_LiftActor::IsMoving() const
{
	return bIsMoving;
}
