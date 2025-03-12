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
	virtual void Detect();		// ����
	virtual void Attack();		// ����
	virtual void Navigate();	// ����
	virtual void Signal();		// ��ȣ
	virtual void HitByDoor();	// ���� �ε���
	virtual void Stun();		// ����
	virtual void WakeUp();		// ���
	virtual void ReceiveDamage();	// �ǰ�

private:
	UPROPERTY(VisibleAnywhere, Category = "FSM");
	class UFSMComponent* fsm;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 hp = 1;

	// Patrol ���� ����
	bool bMovingForward = true;	// �̵� ���� (true: ������, false: ����)
	bool bIsMoving = false;		// ���� �̵� ������ ����
	bool bIsRotating = false;	// ȸ�� ������ ����
	float speed = 0.2f;			// ���� �ӵ�
	float rotatingSpeed = 1.0f;	// ȸ�� �ӵ�
	FVector targetLoc;			// �̵� ��ǥ ����
	FRotator targetRot;			// ��ǥ ȸ�� ��
};
