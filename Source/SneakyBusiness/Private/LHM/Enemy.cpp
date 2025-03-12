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

	// ���� �� ȸ�� ����
	if (bIsRotating)
	{
		FRotator currrentRot = GetActorRotation();
		FRotator newRot = FMath::RInterpTo(currrentRot, targetRot, DeltaTime, rotatingSpeed);

		SetActorRotation(newRot);

		// ��ǥ ȸ���� ���� �����ϸ� ����
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
		// ��ǥ ��ġ ����
		targetLoc = GetActorLocation() + FVector((bMovingForward ? 500.0f : -500.0f), 0, 0);
		bIsMoving = true;
	}

	if(bIsRotating) return; // ȸ�� ���̸� �̵����� ����

	FVector currentLoc = GetActorLocation();
	FVector dir = (targetLoc - currentLoc).GetSafeNormal();
	float dist = FVector::Dist(currentLoc, targetLoc);

	// ��ǥ �������� ���� �������� �ʾ����� �̵�
	if (dist > 1.0f)
	{
		AddMovementInput(dir, speed);
	}
	// ��ǥ ������ �����ϸ� ���� ����
	else
	{
		bMovingForward = !bMovingForward;
		bIsMoving = false;
		bIsRotating = true;

		// ���⿡ ���� Z�� ȸ���� ����
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
	// 3�� �� ������� ���� ����
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