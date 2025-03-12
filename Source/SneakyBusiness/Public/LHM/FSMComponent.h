#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

// FSM 상태 정의
UENUM()
enum class EnemyState : uint8
{
	Patrol		UMETA(DisplayName = "Patrolling"),
	Detect		UMETA(DisplayName = "Detecting"),
	Attack		UMETA(DisplayName = "Attacking"),
	Navigate	UMETA(DisplayName = "Navigating"),
	Signal		UMETA(DisplayName = "Signaling"),
	HitByDoor	UMETA(DisplayName = "Hit by Door"),
	Stun		UMETA(DisplayName = "Stunned"),
	WakeUp		UMETA(DisplayName = "Waking Up")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNEAKYBUSINESS_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFSMComponent();

	void SetState(EnemyState NewState);
	EnemyState GetState() const { return CurrentState; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleState();

private:
	class AEnemy* enemy;

	UPROPERTY(VisibleAnywhere, Category = "FSM")
	EnemyState CurrentState;
};
