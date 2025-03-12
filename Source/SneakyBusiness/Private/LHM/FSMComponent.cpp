#include "LHM/FSMComponent.h"
#include "LHM/Enemy.h"

UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentState = EnemyState::Patrol;
}

void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	enemy = Cast<AEnemy>(GetOwner());
}

void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 현재 상태에 따라 행동 실행
	if (enemy) HandleState();
}

void UFSMComponent::SetState(EnemyState NewState)
{
	CurrentState = NewState;
}

void UFSMComponent::HandleState()
{
	if (!enemy) return;

	switch (CurrentState)
	{
		case EnemyState::Patrol:
			enemy->Patrol();
			break;
		case EnemyState::Detect:
			enemy->Detect();
			break;
		case EnemyState::Attack:
			enemy->Attack();
			break;
		case EnemyState::Navigate:
			enemy->Navigate();
			break;
		case EnemyState::Signal:
			enemy->Signal();
			break;
		case EnemyState::HitByDoor:
			enemy->HitByDoor();
			break;
		case EnemyState::Stun:
			enemy->Stun();
			break;
		case EnemyState::WakeUp:
			enemy->WakeUp();
			break;
		default:
			break;
	}
}