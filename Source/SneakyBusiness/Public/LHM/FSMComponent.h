#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

// FSM 상태 정의
UENUM()
enum class EEnemyState : uint8
{
	Patrol		UMETA(DisplayName = "Patrolling"),
	Attack		UMETA(DisplayName = "Attacking"),
	Chase		UMETA(DisplayName = "Chasing"),
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

	void SetState(EEnemyState NewState);
	EEnemyState GetState() const { return CurrentState; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleState();

private:
	class AEnemy* Enemy;

	UPROPERTY(VisibleAnywhere, Category = "FSM")
	EEnemyState CurrentState;
};
