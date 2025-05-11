// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	Patrol				UMETA(DisplayName = "Patrol"),
	Chase				UMETA(DisplayName = "Chase"),
	Attack				UMETA(DisplayName = "Attack"),
	//Signal				UMETA(DisplayName = "Signal"),
	Stunned				UMETA(DisplayName = "Stunned"),
	Alerted				UMETA(DisplayName = "Alerted")
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
	class UBlackboardData* BBD;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BT;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* CombatSubtree;

	UPROPERTY(VisibleAnywhere, Category = "AI");
	class UAIPerceptionComponent* AIPerceptionComp;
	
	UPROPERTY(VisibleAnywhere, Category = "AI");
	class UAISenseConfig_Sight* SightConfig; // 시각 기반 감지

// Method
	virtual void Attack();		// 공격
	virtual void Signal();		// 신호

	void HitByDoor();			// 문에 부딪힘
	void ReceiveDamage();		// 피격

	// 함정 발동시 해당 위치로 이동
	void ReactToTrapAlert(FVector InAlertLocation);

// Getter/Setter
	EEnemyAIState GetEnemyAIState() const { return CurrentState; }
	void SetEnemyAIState(EEnemyAIState NewState) { CurrentState = NewState; }

	bool IsRotating() { return bIsRotating; }
	bool IsMovingForward() { return bMovingForward; }
	void StartRotating() { bIsRotating = true; }
	void SetTargetRotation(FRotator NewRotation) { TargetRot = NewRotation; }
	FVector GetAlertLocation() { return AlertLocation; }

	bool IsASpace() { return bIsASpace; }
	bool IsMovingDepth() { return bIsMovingDepth; }
	void StartMovingDepth() { bIsMovingDepth = true; }
	void SetMoveDepthLocation(FVector NewLocation) { MoveDepthLocation = NewLocation; }

public:
	void StartMoveToOtherSpace(float YOffset);

	bool IsObstacleAhead(FVector DirectionToDetect,float Distance); // 장애물 판별

	bool IsPlayerStateToFrozenOrDead();		// 플레이어 상태 체크
	bool ShouldDetectHiddenPlayer();		// 플레이어가 숨은 방향에 따라서 감지 여부 판단

protected:
	void LerpRotation(float DeltaTime);		// 순찰 중 회전
	void LerpMoveToDepth(float DeltaTime);	// 추적 중 공간 이동

	UPROPERTY(VisibleAnywhere, Category = "AI");
	EEnemyAIState CurrentState = EEnemyAIState::Patrol;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UMH_ShootComp* ShootComp;

private:
	// Chase
	bool bIsASpace = true;		// 앞공간(A) = true, 뒤공간(B) = false
	bool bIsMovingDepth = false;
	FVector MoveDepthLocation;

	// Patrol
	bool bMovingForward = false;// 이동 방향 (true: 오른쪽, false: 왼쪽)
	bool bIsRotating = false;	// 회전 중인지 여부
	FRotator TargetRot;			// 목표 회전 값
	
	// Stun
	bool bIsStunned = false;
	
	// Alert
	FVector AlertLocation;
};
