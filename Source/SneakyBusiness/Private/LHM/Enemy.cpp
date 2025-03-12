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

	// ���� �� ȸ�� ����
	if (bIsRotating) LerpRotation(DeltaTime);

	// ���� �� �÷��̾� ���� ����
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
		// ��ǥ ��ġ ����
		targetLoc = GetActorLocation() + FVector((bMovingForward ? 500.0f : -500.0f), 0, 0);
		bIsMoving = true;
	}

	// ȸ�� ���̸� �̵����� ����
	if(bIsRotating) return;
	
	FVector currentLoc = GetActorLocation();
	FVector dir = (targetLoc - currentLoc).GetSafeNormal();
	float dist = FVector::Dist(currentLoc, targetLoc);

	// ��ǥ �������� ���� �������� �ʾ����� �̵�
	if (dist > 1.0f)
	{
		// @Todo: ��ֹ��� ������ ���� ��ȯ


		AddMovementInput(dir, speed);
	}
	// ��ǥ ������ �����ϸ� ���� ����
	else
	{
		bMovingForward = !bMovingForward;
		bIsMoving = false;

		// ���⿡ ���� Z�� ȸ���� ����
		float rotationAmount = bMovingForward ? -180.0f : 180.0f;
		targetRot = GetActorRotation() + FRotator(0, rotationAmount, 0);
		bIsRotating = true;
	}
}

void AEnemy::Attack()
{
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is attacking!"));
	UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));

	// @Todo: ���� �Լ� ȣ��
	//
	// @Todo: ��
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

bool AEnemy::DetectPlayer()
{
	// @Todo: �÷��̾ �� �ڿ� ������ ���� ��� -> ���� ���·� ��ȯ

	FVector start = GetActorLocation();
	FVector forwardVec = GetActorForwardVector();
	FVector end = start + (forwardVec * 300.0f);

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this); // ���ʹ� ����

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

	// ��ǥ ȸ���� ���� �����ϸ� ����
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