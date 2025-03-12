#include "LHM/Enemy.h"
#include "LHM/FSMComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	fsm = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComponent"));
	bMovingForward = true;
	bIsMoving = false;
	bIsRotating = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 순찰 중 회전 보간
	if (bIsRotating)
	{
		FRotator currrentRot = GetActorRotation();
		FRotator newRot = FMath::RInterpTo(currrentRot, targetRot, DeltaTime, rotatingSpeed);

		SetActorRotation(newRot);

		// 목표 회전에 거의 도달하면 종료
		if (FMath::Abs(newRot.Yaw - targetRot.Yaw) < 1.0f)
		{
			SetActorRotation(targetRot);
			bIsRotating = false;
		}
	}
}

void AEnemy::Patrol()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is patrolling."));
	if (!bIsMoving && !bIsRotating)
	{
		// 목표 위치 설정
		targetLoc = GetActorLocation() + FVector((bMovingForward ? 500.0f : -500.0f), 0, 0);
		bIsMoving = true;
	}

	if(bIsRotating) return; // 회전 중이면 이동하지 않음

	FVector currentLoc = GetActorLocation();
	FVector dir = (targetLoc - currentLoc).GetSafeNormal();
	float dist = FVector::Dist(currentLoc, targetLoc);

	// 목표 지점까지 아직 도달하지 않았으면 이동
	if (dist > 1.0f)
	{
		AddMovementInput(dir, speed);
	}
	// 목표 지점에 도달하면 방향 반전
	else
	{
		bMovingForward = !bMovingForward;
		bIsMoving = false;
		bIsRotating = true;

		// 방향에 따라 Z축 회전값 설정
		float rotationAmount = bMovingForward ? -180.0f : 180.0f;
		targetRot = GetActorRotation() + FRotator(0, rotationAmount, 0);
		
		//FRotator newRot = GetActorRotation();
		//newRot.Yaw += 180.0f;
		//SetActorRotation(newRot);
	}
}

void AEnemy::Detect()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy detected the player!"));
	fsm->SetState(EnemyState::Attack);
}

void AEnemy::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));
}

void AEnemy::Navigate()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is chasing the player!"));
}

void AEnemy::Signal()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is signaling nearby enemies!"));
}

void AEnemy::HitByDoor()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy was hit by a door and is stunned!"));
	fsm->SetState(EnemyState::Stun);
}

void AEnemy::Stun()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is stunned!"));

	hp = 1;
	// 3초 뒤 깨어남으로 상태 변경
	fsm->SetState(EnemyState::WakeUp);
}

void AEnemy::WakeUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy has woken up!"));
	fsm->SetState(EnemyState::Patrol);
}

void AEnemy::ReceiveDamage()
{
	hp -= 1;
	if (hp <= 0) fsm->SetState(EnemyState::Stun);
}