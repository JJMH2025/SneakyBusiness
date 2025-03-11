// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_Bullet.h"

// Sets default values
AMH_Bullet::AMH_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMH_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMH_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

