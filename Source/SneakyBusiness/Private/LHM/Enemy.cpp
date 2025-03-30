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

	// ���� �� ȸ�� ����
	if (bIsRotating) LerpRotation(DeltaTime);

	// �÷��̾� ����
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
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is attacking!"));
	UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));

	// @Todo: �߻� 3�� ����

	// �÷��̾� ���� ���ϸ� �������·� ��ȯ
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
	// @Todo: �÷��̾ �� �ڿ� ������ ���� ��� -> ���� ���·� ��ȯ

	FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
	FVector ForwardVec = GetActorForwardVector();
	FVector End = Start + (ForwardVec * 300.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // ���ʹ� ����

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

	// ��ǥ ȸ���� ���� �����ϸ� ����
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