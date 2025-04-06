// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_Door.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Components/BoxComponent.h"

// Sets default values
AMH_Door::AMH_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComp
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent=BoxComponent;
	
	//회전 애니메이션
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootScene->SetupAttachment(RootComponent);

	// Door Mesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootScene);
	
}

// Called when the game starts or when spawned
void AMH_Door::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMH_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotatingDoor)
	{
		//현재 회전값
		float CurrentYaw = RootScene->GetComponentRotation().Yaw;
		// 최단 거리 회전각 계산
		float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
		float NewYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + DeltaYaw, GetWorld()->GetDeltaSeconds(), 15.0f);
	
		RootScene->SetRelativeRotation(FRotator(0.f,NewYaw,0.f));

		if (FMath::Abs(NewYaw - TargetYaw) < 1.0f)
		{
			NewYaw = TargetYaw;
			bIsRotatingDoor = false;
			//지나갔는지 확인하고 닫히기
		}
	}
}

void AMH_Door::DoorOpen(const float LastDirection)
{
	GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Turquoise,TEXT("DoorOpen"));
	//문열리기
	bIsRotatingDoor = true;

	//오른쪽
	if (LastDirection == 1)
	{
		TargetYaw = -90.f;
	}

	//왼쪽
	else if (LastDirection == -1)
	{
		TargetYaw = 90.f;
	}
}

void AMH_Door::DoorClosed()
{
	bIsRotatingDoor = true;
	GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Turquoise,TEXT("DoorClosed"));
	//문닫히기
	TargetYaw = 0.f;
	//회전값 원래 위치로 돌아가기
}

