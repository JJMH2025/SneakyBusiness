// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_ShootComp.h"

// Sets default values for this component's properties
UMH_ShootComp::UMH_ShootComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMH_ShootComp::BeginPlay()
{
	Super::BeginPlay();
	//재장전
	AmmoSetUp();
	// ...
	
}


// Called every frame
void UMH_ShootComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMH_ShootComp::AmmoSetUp()
{
	CurrentAmmo = MaxAmmo;
}

void UMH_ShootComp::Shooting(FVector FireLocation, FRotator FireRotation)
{
	if (!bCanShoot || CurrentAmmo <= 0)
	{
		return;
	}
		if (CurrentAmmo > 0 && CurrentAmmo <= MaxAmmo)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("Shoot!"));
			//총알 -1
			CurrentAmmo--;

			bCanShoot = false;

			//일정 시간 지나야 가능
			GetWorld()->GetTimerManager().SetTimer(ShootTimer, this, &UMH_ShootComp::ResetShoot, ShootCooldown, false);
		}
	if (CurrentAmmo <= 0)
	{
		CurrentAmmo = 0;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), CurrentAmmo));
}

void UMH_ShootComp::ReloadAmmo()
{
	if (CurrentAmmo < MaxAmmo)
	{
		CurrentAmmo++;
	}
}

void UMH_ShootComp::ResetShoot()
{
	bCanShoot = true;
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green,TEXT("bCanShoot true"));
}

