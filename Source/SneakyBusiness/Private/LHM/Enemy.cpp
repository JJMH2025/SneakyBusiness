#include "LHM/Enemy.h"
#include "LHM/FSMComponent.h"
#include "MH/MH_ShootComp.h"
#include "MH/Player_Nick.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	fsm = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComp"));
	shoot = CreateDefaultSubobject<UMH_ShootComp>(TEXT("ShootComp"));

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

	// 순찰 중 플레이어 감지 실행
	if (bIsPatrolling && DetectPlayer())
	{
		bIsPatrolling = false;
		fsm->SetState(EnemyState::Attack);
	}


}

void AEnemy::Patrol()
{
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is patrolling."));
	UE_LOG(LogTemp, Warning, TEXT("Enemy is patrolling."));
	bIsPatrolling = true;

	if (!bIsMoving && !bIsRotating)
	{
		// 목표 위치 설정
		targetLoc = GetActorLocation() + FVector((bMovingForward ? 500.0f : -500.0f), 0, 0);
		bIsMoving = true;
	}

	// 회전 중이면 이동하지 않음
	if(bIsRotating) return;
	
	FVector currentLoc = GetActorLocation();
	FVector dir = (targetLoc - currentLoc).GetSafeNormal();
	float dist = FVector::Dist(currentLoc, targetLoc);

	// 목표 지점까지 아직 도달하지 않았으면 이동
	if (dist > 1.0f)
	{
		// @Todo: 장애물에 막히면 방향 전환


		AddMovementInput(dir, speed);
	}
	// 목표 지점에 도달하면 방향 반전
	else
	{
		bMovingForward = !bMovingForward;
		bIsMoving = false;

		// 방향에 따라 Z축 회전값 설정
		float rotationAmount = bMovingForward ? -180.0f : 180.0f;
		targetRot = GetActorRotation() + FRotator(0, rotationAmount, 0);
		bIsRotating = true;
	}
}

void AEnemy::Attack()
{
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is attacking!"));
	UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));

	// @Todo: 공격 함수 호출
	//
	// @Todo: 플
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

bool AEnemy::DetectPlayer()
{
	// @Todo: 플레이어가 벽 뒤에 숨으면 감지 취소 -> 순찰 상태로 전환

	FVector start = GetActorLocation();
	FVector forwardVec = GetActorForwardVector();
	FVector end = start + (forwardVec * 300.0f);

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this); // 에너미 제외

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Pawn, params);

	DrawDebugLine(GetWorld(), start, end, bHit ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);

	if (bHit)
	{
		AActor* hitActor = hitResult.GetActor();
		//if(hitActor && hitActor->IsA(APlayer_Nick::StaticClass()))
		if(hitActor && hitActor->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy detected the player!"));
			UE_LOG(LogTemp, Warning, TEXT("Enemy detected the player!"));
			return true;
		}
	}
	return false;
}

void AEnemy::LerpRotation(float deltaTime)
{
	FRotator currrentRot = GetActorRotation();
	FRotator newRot = FMath::RInterpTo(currrentRot, targetRot, deltaTime, rotatingSpeed);

	SetActorRotation(newRot);

	// 목표 회전에 거의 도달하면 종료
	if (FMath::Abs(newRot.Yaw - targetRot.Yaw) < 1.0f)
	{
		SetActorRotation(targetRot);
		bIsRotating = false;
	}
}

void AEnemy::ReceiveDamage()
{
	hp -= 1;
	if (hp <= 0) fsm->SetState(EnemyState::Stun);
}