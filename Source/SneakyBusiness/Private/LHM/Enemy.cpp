#include "LHM/Enemy.h"
#include "LHM/FSMComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	fsm = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComponent"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::Patrol()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is patrolling."));
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