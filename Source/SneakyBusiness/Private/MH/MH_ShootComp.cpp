// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_ShootComp.h"

#include "LHM/Enemy.h"
#include "MH/MH_Bullet.h"
#include "MH/Player_Nick.h"

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
	// ...
	// Owner 설정
	ShootCompOwner = this->GetOwner();
	
	//재장전
	if (ShootCompOwner!=nullptr && ShootCompOwner->IsA(APlayer_Nick::StaticClass()))
	{
		AmmoSetUp();
	}
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
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("AmmoSetUp"));
}

void UMH_ShootComp::Shooting(FVector FireLocation, FRotator FireRotation)
{
	//플레이어가 쏜 경우
	if (ShootCompOwner!=nullptr && ShootCompOwner->IsA(APlayer_Nick::StaticClass()))
	{
		if (!bCanShoot || CurrentAmmo <= 0)
		{
			return;
		}
		if (CurrentAmmo > 0 && CurrentAmmo <= MaxAmmo)
		{
			//총알 생성
			if (bulletFactory)
			{
				AMH_Bullet* Bullet = GetWorld()->SpawnActor<AMH_Bullet>(bulletFactory, FireLocation + FVector(70, 0, 0),
				                                                        FireRotation);
				if (Bullet)
				{
					//총알에 플레이어를 발사자로 설정
					Bullet->SetOwnerActor(ShootCompOwner);
				}
			}
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
	//에너미가 쏜 경우
	if (ShootCompOwner!=nullptr && ShootCompOwner->IsA(AEnemy::StaticClass()))
	{
		//조건 검사 후 맞다면 FireLocation, FireRotation는.. 에너미 총위치로?
		//총알 생성
		if (bulletFactory)
		{
			AMH_Bullet* Bullet = GetWorld()->SpawnActor<AMH_Bullet>(bulletFactory, FireLocation + FVector(70, 0, 0),
																	FireRotation);
			if (Bullet)
			{
				//총알에 플레이어를 발사자로 설정
				Bullet->SetOwnerActor(ShootCompOwner);
			}
		}
	}
}

void UMH_ShootComp::ReloadAmmo()
{
	if (CurrentAmmo < MaxAmmo)
	{
		CurrentAmmo++;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"), CurrentAmmo));
	
}

void UMH_ShootComp::ResetShoot()
{
	bCanShoot = true;
}
