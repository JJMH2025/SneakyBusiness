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

	// ���� �� ȸ�� ����
	if (bIsRotating) LerpRotation(DeltaTime);

	// �̵� ���� ����
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

	// �÷��̾� ����
	if (!bIsRotating)
	{
		if (IsPlayerDetected())
		{
			Fsm->SetState(EEnemyState::Chase);
		}
	}

	if (!bIsMoving && !bIsRotating)
	{
		// ��ǥ ��ġ ����
		TargetLoc = GetActorLocation() + FVector((bMovingForward ? 500.0f : -500.0f), 0, 0);
		bIsMoving = true;
	}

	// ȸ�� ���̸� �̵����� ����
	if(bIsRotating) return;

	FVector CurrentLoc = GetActorLocation();
	FVector Dir = (TargetLoc - CurrentLoc).GetSafeNormal();
	float Dist = FVector::Dist(CurrentLoc, TargetLoc);

	// ��ǥ �������� ���� �������� �ʾ����� �̵�
	if (Dist > 1.0f)
	{
		AddMovementInput(Dir, Speed);
	}
	// ��ǥ ������ �����ϸ� ���� ����
	else
	{
		bMovingForward = !bMovingForward;
		bIsMoving = false;

		// ���⿡ ���� Z�� ȸ���� ����
		float RotationAmount = bMovingForward ? -180.0f : 180.0f;
		TargetRot = GetActorRotation() + FRotator(0, RotationAmount, 0);
		bIsRotating = true;
	}
}

void AEnemy::Attack()
{
	//GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is attacking!"));
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));

	// �÷��̾� ���� ���ϸ� �������·� ��ȯ
	if (!IsPlayerDetected())
	{
		Fsm->SetState(EEnemyState::Patrol);
	}

	if (!bAttackStarted)
	{
		bAttackStarted = true;
		// 1�� �� �Ѿ� �߻�
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
		// �÷��̾� �������� �̵�
		FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		AddMovementInput(Direction, 0.5f);

		// �÷��̾���� �Ÿ� üũ (300 �����̸� Attack ���·� ��ȯ)
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
	// 3�� �� ������� ���� ����
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
	Params.AddIgnoredActor(this); // ���ʹ� ����

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

	// ��ǥ ȸ���� ���� �����ϸ� ����
	if (FMath::Abs(NewRot.Yaw - TargetRot.Yaw) < 1.0f)
	{
		SetActorRotation(TargetRot);
		bIsRotating = false;
	}
}

void AEnemy::MoveSideways(float OffsetY)
{
	// ���� ȸ�� �� ����
	float InitialYaw = GetActorRotation().Yaw;

	// ���� -90�� ȸ�� ����
	TargetYaw = InitialYaw = 90.0f;

	// �̵� ��ǥ ����
	SideTargetLocation = GetActorLocation() + FVector(0, OffsetY, 0);
	bIsMovingSideways = true;
}

void AEnemy::DoShooting()
{
	// �Ѿ� �߻�
	if (ShootComp)
	{
		FVector FireLocation = GetActorLocation();
		FRotator FireRotation = GetActorRotation();
		ShootComp->Shooting(FireLocation, FireRotation);
	}

	// ���� �� �ٽ� Chase ���·� ��ȯ�ؼ� �÷��̾ ����
	bAttackStarted = false;
	Fsm->SetState(EEnemyState::Patrol);
}

bool AEnemy::IsPlayerStateToFrozenOrDead()
{
	// MH��û : �÷��̾ Frozen, Dead�� �� ���� ���ϰ� ����
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