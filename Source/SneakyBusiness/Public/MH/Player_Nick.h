// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SneakyBusiness/SneakyBusinessCharacter.h"
#include "Player_Nick.generated.h"

UCLASS()
class SNEAKYBUSINESS_API APlayer_Nick : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Nick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//메쉬
	
	//카메라
	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* CameraComp;
	
	
	//플레이어 이동
	UFUNCTION()
	void MoveHorizontal(const FInputActionValue& Value); //좌우 이동 AD
	UFUNCTION()
	void MoveDepth(const FInputActionValue& Value); //앞뒤 이동 WS
	UFUNCTION()
	void JumpNick();

	//현재 앞,뒤 동간위치 (앞 : true, 뒤: false)
	bool bIsPlayerLoc = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Input)
	float ALoc = 0.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Input)
	float BLoc = -100.f;
	
	float TargetYawLot; // 목표 이동 위치
	bool bIsMovingDepth; // 이동 중인지 확인
	
	bool bIsRotating; // 이동 중이며 회전 중인지 확인
	float TargetYaw; // 목표 회전 값
	float LastHorizontalDirection; // 마지막으로 바라본 좌우 방향 (1: 오른쪽, -1: 왼쪽)

	//공격
	UFUNCTION()
	void Shoot();
	
	//IA
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAMoveHorizontal;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAMoveDepth;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAJump;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAShoot;

	//총알
		
};
