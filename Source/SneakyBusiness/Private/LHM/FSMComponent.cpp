#include "LHM/FSMComponent.h"
#include "LHM/Enemy.h"

UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentState = EEnemyState::Patrol;
}

void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetOwner());
}

void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 현재 상태에 따라 행동 실행
	if (Enemy) HandleState();
}

void UFSMComponent::SetState(EEnemyState NewState)
{
	CurrentState = NewState;
}

void UFSMComponent::HandleState()
{
	if (!Enemy) return;

	switch (CurrentState)
	{
		case EEnemyState::Patrol:
			Enemy->Patrol();
			break;
		case EEnemyState::Attack:
			Enemy->Attack();
			break;
		case EEnemyState::Chase:
		case EEnemyState::MoveToDepth:
			Enemy->HandleChaseExtended(CurrentState);
			break;
		case EEnemyState::Signal:
			Enemy->Signal();
			break;
		case EEnemyState::HitByDoor:
			Enemy->HitByDoor();
			break;
		case EEnemyState::Stun:
			Enemy->Stun();
			break;
		case EEnemyState::WakeUp:
			Enemy->WakeUp();
			break;
		default:
			break;
	}
}