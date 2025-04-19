// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_Lasertrap.h"

// Sets default values
AMH_Lasertrap::AMH_Lasertrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMH_Lasertrap::BeginPlay()
{
	Super::BeginPlay();

	Directions = {
		FVector(0, 0, 1),    // Z+ (앞)
		FVector(0, 0, -1),   // Z- (뒤)
		FVector(1, 0, 0)};     // X 방향
	
}

// Called every frame
void AMH_Lasertrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMH_Lasertrap::FireLaser()
{
	//레이저 시작 위치
	FVector LaserStart = GetActorLocation();

	//현재 방향
	FVector Direction = Directions[CurrentDirIdx];

	//레이저 끝나는 위치 = 시작점 + 방향 * 길이
	FVector LaserEnd = LaserStart + Direction*LaserLength;

	FHitResult LaserHit;

	//자기자신 무시
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//라인트레이스로 Pawn 채널에서 감지
	bool bHit = GetWorld()->LineTraceSingleByChannel(LaserHit,LaserStart,LaserEnd,ECC_Pawn,Params);

	//맞은 대상이 플레이어일 경우
	if (bHit && LaserHit.GetActor()->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Hit by Laser!");

		//AlertNearbyEnemies(LaserHit.GetActor());
	}

	//레이저 선
	DrawDebugLine(GetWorld(),LaserStart,LaserEnd,FColor::Red,false,1.0f,0,3.0f);

	//다음 방향 인덱스로 순환
	CurrentDirIdx = (CurrentDirIdx + 1) % Directions.Num();

	//다음 발사 예약
	GetWorldTimerManager().SetTimer(LaserTimerHandle,this,&AMH_Lasertrap::FireLaser,LaserInterval);
}
