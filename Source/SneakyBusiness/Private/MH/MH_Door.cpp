// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_Door.h"

// Sets default values
AMH_Door::AMH_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}

void AMH_Door::DoorOpen(const FVector& InstigatorForward)
{
	GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Turquoise,TEXT("DoorOpen"));
	//문열리기
	//오른쪽
	//왼쪽
}

void AMH_Door::DoorClosed()
{
	GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Turquoise,TEXT("DoorClosed"));
	//문닫히기
	//회전값 원래 위치로 돌아가기
}

