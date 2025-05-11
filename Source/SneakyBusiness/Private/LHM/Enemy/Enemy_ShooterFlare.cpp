// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/Enemy/Enemy_ShooterFlare.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MH/MH_ShootComp.h"
#include "MH/MH_EnemyAlertComp.h"

AEnemy_ShooterFlare::AEnemy_ShooterFlare()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/LHM/BluePrints/AI/BT_Combat_ShooterFlare.BT_Combat_ShooterFlare"));
	if (BTAssetRef.Succeeded())
	{
		CombatSubtree = BTAssetRef.Object;
	}
}

void AEnemy_ShooterFlare::Attack()
{
	Super::Attack();

	if(!ShootComp) return;

	// 플레이어가 높은 곳에 있으면 감지 불가
	// if(!IsPlayerReachable()) return;

	FVector FireLoc = GetActorLocation();
	FRotator FireRot = GetActorRotation();
	ShootComp->Shooting(FireLoc, FireRot);
}

void AEnemy_ShooterFlare::Signal()
{
	Super::Signal();

	UE_LOG(LogTemp, Log, TEXT("공포탄 발사 - 주변 에너미 호출"));

	// 호루라기 애니메이션 몽타주 재생
	/*if(Whistle)
	{
		PlayAnimMontage(Whistle);
	}*/

	AlertComp->AlertNearbyEnemies(GetActorLocation(), 1000.0f);
}
