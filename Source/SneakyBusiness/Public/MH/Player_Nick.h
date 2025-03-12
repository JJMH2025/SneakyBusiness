// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MH_ShootComp.h"
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
	
public:
	//메쉬


	//컴포넌트들
	//공격
	UPROPERTY(VisibleAnywhere,Category="Components")
	UMH_ShootComp* ShootComp;
	
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

    //죽었다 살아났을 때 초기화 (HP,CurrentAmmo,)
	UFUNCTION()
	void RespawnSetup();
	
	//공격
	UFUNCTION()
	void Shooting();
	
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

	//플레이어 HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	int32 MaxHP = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	int32 PlayerHP = MaxHP;

	//총알
	//총 쏠수 있는지 여부
	//bool bCanShoot = true;
	//쿨타임 타이머
	//FTimerHandle ShootTimer;
	//발사쿨타임함수
	//UFUNCTION()
	//void ResetShoot();
	//발사간격, 쿨타임
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	//float ShootCooldown = .7f;
	// 총알 개수 변수
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	//int32 CurrentAmmo = 2;  // 기본 2발 장전 가능

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	//int32 MaxAmmo = 2;  // 최대 장전 가능 개수

	//총알 장전 함수
	//UFUNCTION()
	//void ReloadAmmo();

	

		
};
