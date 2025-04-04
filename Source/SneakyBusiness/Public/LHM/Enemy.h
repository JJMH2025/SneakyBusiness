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
	virtual void Chase();		// ����
	virtual void Signal();		// ��ȣ
	virtual void HitByDoor();	// ���� �ε���
	virtual void Stun();		// ����
	virtual void WakeUp();		// ���

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	//bool IsPlayerDetected();			// ����
	void LerpRotation(float DeltaTime);	// ���� �� ȸ��
	void MoveSideways(float OffsetY);
	void DoShooting();
	void ReceiveDamage();				// �ǰ�
	
	// �÷��̾� ���� üũ
	bool IsPlayerStateToFrozenOrDead();
	// �������� ���� ���� ���� �Ǵ� (���� �� �÷��̾ ���ƴ��� �Ǵ�)
	bool IsPlayerDetectedByAIPerception();

private:
	UPROPERTY(VisibleAnywhere, Category = "FSM");
	class UFSMComponent* Fsm;

	UPROPERTY(VisibleAnywhere, Category = "FSM");
	class UAIPerceptionComponent* AIPerceptionComp;
	
	UPROPERTY(VisibleAnywhere, Category = "FSM");
	class UAISenseConfig_Sight* SightConfig; // �ð� ��� ����

	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class UMH_ShootComp* ShootComp;

	UPROPERTY(EditDefaultsOnly, Category = "FSM")
	int32 Hp = 1;

	// Patrol ���� ����
	bool bMovingForward = true;	// �̵� ���� (true: ������, false: ����)
	bool bIsMoving = false;		// ���� �̵� ������ ����
	bool bIsRotating = false;	// ȸ�� ������ ����
	float Speed = 0.2f;			// ���� �ӵ�
	FVector TargetLoc;			// �̵� ��ǥ ����
	FRotator TargetRot;			// ��ǥ ȸ�� ��

	// Chase
	bool bIsMovingSideways = false; // �¿� �̵� ������ ����
	FVector SideTargetLocation;		// ��ǥ �̵� ��ġ
	float TargetYaw = 0.0f;			// ��ǥ ȸ�� ��

	// Attack
	FTimerHandle AttackTimerHandle;
	bool bAttackStarted = false;
	
};
