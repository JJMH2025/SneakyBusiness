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

	virtual void Patrol();		// ����
	virtual void Attack();		// ����
	virtual void Navigate();	// ����
	virtual void Signal();		// ��ȣ
	virtual void HitByDoor();	// ���� �ε���
	virtual void Stun();		// ����
	virtual void WakeUp();		// ���

	bool IsPlayerDetected();	// ����
	void ReceiveDamage();		// �ǰ�
	void LerpRotation(float DeltaTime);	// ���� �� ȸ��

private:
	UPROPERTY(VisibleAnywhere, Category = "Custom");
	class UFSMComponent* Fsm;

	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	int32 Hp = 1;

	// Patrol ���� ����
	bool bIsPatrolling = true;
	bool bMovingForward = true;	// �̵� ���� (true: ������, false: ����)
	bool bIsMoving = false;		// ���� �̵� ������ ����
	bool bIsRotating = false;	// ȸ�� ������ ����
	float Speed = 0.2f;			// ���� �ӵ�
	FVector TargetLoc;			// �̵� ��ǥ ����
	FRotator TargetRot;			// ��ǥ ȸ�� ��

	FTimerHandle WakeUpTimer;

	
};
