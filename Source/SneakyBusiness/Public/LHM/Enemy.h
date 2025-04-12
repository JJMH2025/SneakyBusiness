// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	Patrol				UMETA(DisplayName = "Patrol"),
	MovingToAlignX		UMETA(DisplayName = "MovingToAlignX"),
	MovingToOtherSpace	UMETA(DisplayName = "MovingToOtherSpace"),
	Chase				UMETA(DisplayName = "Chase"),
	Attack				UMETA(DisplayName = "Attack"),
	Signal				UMETA(DisplayName = "Signal"),
	HitByDoor			UMETA(DisplayName = "HitByDoor"),
	Stunned				UMETA(DisplayName = "Stunned"),
	WakeUp				UMETA(DisplayName = "WakeUp")
};

UCLASS()
class SNEAKYBUSINESS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Behavior Tree
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BT;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBlackboardData* BBD;
	
	UPROPERTY(VisibleAnywhere, Category = "AI");
	class UAIPerceptionComponent* AIPerceptionComp;
	UPROPERTY(VisibleAnywhere, Category = "AI");
	class UAISenseConfig_Sight* SightConfig; // 시각 기반 감지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI");
	EEnemyAIState CurrentState = EEnemyAIState::Patrol;

	// 상태에 따른 행동 함수
	virtual void Patrol();					// 순찰
	virtual void Chase();					// 추적
	virtual void AlignXToPlayer();			// 추적 시 플레이어 방향으로 X축 먼저 정렬
	virtual void PrepareMoveToOtherSpace(); // 추적 시 X축 정렬 후 A/B 공간 전환
	virtual void Attack();					// 공격
	virtual void Signal();					// 신호
	virtual void HitByDoor();				// 문에 부딪힘
	virtual void Stun();					// 기절
	virtual void WakeUp();					// 깨어남

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	void LerpRotation(float DeltaTime);		// 순찰 중 회전
	void LerpMoveToDepth(float DeltaTime);	// 추적 중 공간 이동
	void DoShooting();						// 총알 발사
	void ReceiveDamage();					// 피격
	void HandleStunEnd();					// 기절 3초 뒤 깨어남 

	// 수동으로 상태 유지 여부 판단 (추적 중 플레이어를 놓쳤는지 판단)
	bool IsPlayerDetectedByAIPerception();
	// 장애물 판별
	bool IsObstacleAhead(float Distance = 100.0f);
	// 플레이어 상태 체크
	bool IsPlayerStateToFrozenOrDead();

private:
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class UMH_ShootComp* ShootComp;

	UPROPERTY(EditDefaultsOnly, Category = "FSM")
	int32 Hp = 1;

	// Chase
	bool bIsASpace = true;	// 앞공간(A) = true, 뒤공간(B) = false
	FVector MoveTargetLocation;
	bool bIsMovingDepth = false;

	// Patrol
	bool bMovingForward = true;	// 이동 방향 (true: 오른쪽, false: 왼쪽)
	bool bIsRotating = false;	// 회전 중인지 여부
	FRotator TargetRot;			// 목표 회전 값

	// Attack
	FTimerHandle AttackTimerHandle;
	bool bAttackStarted = false;
	
	// Stun
	FTimerHandle StunTimerHandle;
	bool bIsStunned = false;
	
};
