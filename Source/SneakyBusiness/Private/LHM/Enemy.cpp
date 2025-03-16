#include "LHM/Enemy.h"
#include "LHM/FSMComponent.h"
#include "MH/MH_ShootComp.h"
#include "MH/Player_Nick.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Fsm = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComp"));

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
	if (bIsRotating) LerpRotation(DeltaTime);

	// 플레이어 감지
	if (!bIsRotating)
	{
		if (IsPlayerDetected())
		{
			bIsPatrolling = false;
			Fsm->SetState(EEnemyState::Attack);
		}
	}

}

void AEnemy::Patrol()
{
	if(!bIsPatrolling) return;

	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is patrolling."));
	UE_LOG(LogTemp, Warning, TEXT("Enemy is patrolling."));
	//bIsPatrolling = true;

	if (!bIsMoving && !bIsRotating)
	{
		// 목표 위치 설정
		TargetLoc = GetActorLocation() + FVector((bMovingForward ? 500.0f : -500.0f), 0, 0);
		bIsMoving = true;
	}

	// 회전 중이면 이동하지 않음
	if(bIsRotating) return;
	
	FVector CurrentLoc = GetActorLocation();
	FVector Dir = (TargetLoc - CurrentLoc).GetSafeNormal();
	float Dist = FVector::Dist(CurrentLoc, TargetLoc);

	// 목표 지점까지 아직 도달하지 않았으면 이동
	if (Dist > 1.0f)
	{
		AddMovementInput(Dir, Speed);
	}
	// 목표 지점에 도달하면 방향 반전
	else
	{
		bMovingForward = !bMovingForward;
		bIsMoving = false;

		// 방향에 따라 Z축 회전값 설정
		float RotationAmount = bMovingForward ? -180.0f : 180.0f;
		TargetRot = GetActorRotation() + FRotator(0, RotationAmount, 0);
		bIsRotating = true;
	}
}

void AEnemy::Attack()
{
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is attacking!"));
	UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));

	// @Todo: 발사 3번 연속

	// 플레이어 감지 못하면 순찰상태로 전환
	if (!IsPlayerDetected())
	{
		bIsPatrolling = true;
		Fsm->SetState(EEnemyState::Patrol);
	}
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
	Fsm->SetState(EEnemyState::Stun);
}

void AEnemy::Stun()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is stunned!"));

	Hp = 1;
	// 3초 뒤 깨어남으로 상태 변경
	Fsm->SetState(EEnemyState::WakeUp);
}

void AEnemy::WakeUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy has woken up!"));
	Fsm->SetState(EEnemyState::Patrol);
}

bool AEnemy::IsPlayerDetected()
{
	// @Todo: 플레이어가 벽 뒤에 숨으면 감지 취소 -> 순찰 상태로 전환

	FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
	FVector ForwardVec = GetActorForwardVector();
	FVector End = Start + (ForwardVec * 300.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 에너미 제외

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params);

	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if(HitActor && HitActor->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy detected the player!"));
			UE_LOG(LogTemp, Warning, TEXT("Enemy detected the player!"));
			return true;
		}
	}
	return false;
}

void AEnemy::LerpRotation(float DeltaTime)
{
	FRotator CurrrentRot = GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrrentRot, TargetRot, DeltaTime, 5.0f);

	SetActorRotation(NewRot);

	// 목표 회전에 거의 도달하면 종료
	if (FMath::Abs(NewRot.Yaw - TargetRot.Yaw) < 1.0f)
	{
		SetActorRotation(TargetRot);
		bIsRotating = false;
	}
}

void AEnemy::ReceiveDamage()
{
	Hp -= 1;
	if (Hp <= 0) Fsm->SetState(EEnemyState::Stun);
}