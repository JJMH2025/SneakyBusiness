#include "LHM/Enemy.h"
#include "LHM/FSMComponent.h"
#include "MH/MH_ShootComp.h"
#include "MH/Player_Nick.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Fsm = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComp"));
	ShootComp = CreateDefaultSubobject<UMH_ShootComp>(TEXT("ShootComp"));

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

	// 이동 보간 적용
	if (bIsMovingSideways)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, SideTargetLocation, DeltaTime, 5.0f);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, SideTargetLocation) < 2.0f)
		{
			SetActorLocation(SideTargetLocation);
			bIsMovingSideways = false;
		}
	}
}

void AEnemy::Patrol()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is patrolling."));

	// 플레이어 감지
	if (!bIsRotating)
	{
		if (IsPlayerDetected())
		{
			Fsm->SetState(EEnemyState::Chase);
		}
	}

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
	//GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is attacking!"));
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));

	// 플레이어 감지 못하면 순찰상태로 전환
	if (!IsPlayerDetected())
	{
		Fsm->SetState(EEnemyState::Patrol);
	}

	if (!bAttackStarted)
	{
		bAttackStarted = true;
		// 1초 후 총알 발사
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::DoShooting, 1.0f, false);
	}
}

void AEnemy::Chase()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is chasing the player!"));

	if (IsPlayerStateToFrozenOrDead()) return;

	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Player)
	{
		// 플레이어 방향으로 이동
		FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		AddMovementInput(Direction, 0.5f);

		// 플레이어와의 거리 체크 (300 이하이면 Attack 상태로 전환)
		if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) <= 300.0f)
		{
			Fsm->SetState(EEnemyState::Attack);
		}
	}
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
	if(IsPlayerStateToFrozenOrDead()) return;

	FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);

	FVector ForwardVec = GetActorForwardVector();
	FVector LeftVec = ForwardVec.RotateAngleAxis(-45, FVector(0, 0, 1));
	FVector RightVec = ForwardVec.RotateAngleAxis(45, FVector(0, 0, 1));

	FVector End = Start + (ForwardVec * 500.0f);
	FVector EndLeft = Start + (LeftVec * 500.0f);
	FVector EndRight = Start + (RightVec * 500.0f);

	FHitResult HitResult, HitResultLeft, HitResultRight;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 에너미 제외

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params);
	bool bHitLeft = GetWorld()->LineTraceSingleByChannel(HitResultLeft, Start, EndLeft, ECC_Pawn, Params);
	bool bHitRight = GetWorld()->LineTraceSingleByChannel(HitResultRight, Start, EndRight, ECC_Pawn, Params);

	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);
	//DrawDebugLine(GetWorld(), Start, EndLeft, bHitLeft ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);
	//DrawDebugLine(GetWorld(), Start, EndRight, bHitRight ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if(HitActor && HitActor->ActorHasTag("Player"))
		{
			return true;
		}
	}

	/*if (bHitLeft)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->ActorHasTag("Player"))
		{
			MoveSideways(-100.0f);
			return true;
		}
	}

	if (bHitRight)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->ActorHasTag("Player"))
		{
			MoveSideways(100.0f);
			return true;
		}
	}*/

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

void AEnemy::MoveSideways(float OffsetY)
{
	// 현재 회전 값 저장
	float InitialYaw = GetActorRotation().Yaw;

	// 먼저 -90도 회전 시작
	TargetYaw = InitialYaw = 90.0f;

	// 이동 목표 설정
	SideTargetLocation = GetActorLocation() + FVector(0, OffsetY, 0);
	bIsMovingSideways = true;
}

void AEnemy::DoShooting()
{
	// 총알 발사
	if (ShootComp)
	{
		FVector FireLocation = GetActorLocation();
		FRotator FireRotation = GetActorRotation();
		ShootComp->Shooting(FireLocation, FireRotation);
	}

	// 공격 후 다시 Chase 상태로 전환해서 플레이어를 추적
	bAttackStarted = false;
	Fsm->SetState(EEnemyState::Patrol);
}

bool AEnemy::IsPlayerStateToFrozenOrDead()
{
	// MH요청 : 플레이어가 Frozen, Dead일 때 감지 못하게 설정
	APlayer_Nick* Player = Cast<APlayer_Nick>(GetOwner());
	if (Player)
	{
		EPlayerState PlayerState = Player->CurrentPlayerState;
		if (PlayerState == EPlayerState::Frozen || PlayerState == EPlayerState::Invincible)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void AEnemy::ReceiveDamage()
{
	Hp -= 1;
	if (Hp <= 0) Fsm->SetState(EEnemyState::Stun);
}