// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

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

	virtual void Patrol();		// 순찰
	virtual void Attack();		// 공격
	virtual void Navigate();	// 추적
	virtual void Signal();		// 신호
	virtual void HitByDoor();	// 문에 부딪힘
	virtual void Stun();		// 기절
	virtual void WakeUp();		// 깨어남

	bool IsPlayerDetected();	// 감지
	void ReceiveDamage();		// 피격
	void LerpRotation(float DeltaTime);	// 순찰 중 회전

private:
	UPROPERTY(VisibleAnywhere, Category = "Custom");
	class UFSMComponent* Fsm;

	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	int32 Hp = 1;

	// Patrol 관련 변수
	bool bIsPatrolling = true;
	bool bMovingForward = true;	// 이동 방향 (true: 오른쪽, false: 왼쪽)
	bool bIsMoving = false;		// 현재 이동 중인지 여부
	bool bIsRotating = false;	// 회전 중인지 여부
	float Speed = 0.2f;			// 순찰 속도
	FVector TargetLoc;			// 이동 목표 지점
	FRotator TargetRot;			// 목표 회전 값

	FTimerHandle WakeUpTimer;

	
};
