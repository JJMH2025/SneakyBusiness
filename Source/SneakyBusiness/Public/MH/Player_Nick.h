// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MH_ShootComp.h"
#include "GameFramework/Character.h"
#include "SneakyBusiness/SneakyBusinessCharacter.h"
#include "Player_Nick.generated.h"

//플레이어 상태 enum
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Normal,
	Frozen,
	Invincible,
	Dead
};

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
//Test/////////////////////////////////
	UFUNCTION(Blueprintable)
	void TestF();

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IATest1;
///////////////////////////////////
	
	
	//메쉬

	//컴포넌트들
	//콜리전 오버랩 체크
	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//공격
	UPROPERTY(VisibleAnywhere,Category="Components")
	UMH_ShootComp* ShootComp;
	
	//카메라
	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* CameraComp;

	
	//플레이어 상태 변경
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="MyPlayerSettings")
	EPlayerState CurrentPlayerState = EPlayerState::Normal;

	FTimerHandle FrozenTimerHandle;
	FTimerHandle InvincibleTimerHandle;

	// Frozen 기절상태 호출
	UFUNCTION()
	void StartFrozen();
	
	//Invincible 무적상태 호출
	UFUNCTION()
	void StartInvincible();
	//Normal 일반상태 호출
	UFUNCTION()
	void ResetToNormal();
	//물건 떨굼
	UFUNCTION()
	void DropItems();
	
	//플레이어 이동
	UFUNCTION()
	void MoveHorizontal(const FInputActionValue& Value); //좌우 이동 AD
	UFUNCTION()
	void StopHorizontal(const FInputActionValue& Value); //좌우 이동 AD
	UFUNCTION()
	void MoveDepth(const FInputActionValue& Value); //앞뒤 이동 WS
	UFUNCTION()
	void JumpNick();

	//플레이어 상호작용
	UFUNCTION()
	void PlayerInteract();

	UPROPERTY()
	bool bIsOverlapDoor = false;
	UPROPERTY()
	class AMH_Door* OverlapDoor = nullptr;
	

	UPROPERTY()
	class UArrowComponent* ShootArrowComp;

	//현재 앞,뒤 동간위치 (앞 : true, 뒤: false)
	bool bIsPlayerLoc = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	float ALoc = 0.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
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

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IAInteract;

	//플레이어 HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	int32 MaxHP = 4;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	int32 CurrentPlayerHP = MaxHP;

	//데미지
	UFUNCTION()
	void PlayerTakeDamage();
	//사망
	UFUNCTION()
	void PlayerDie();
	//숨기
	UFUNCTION()
	void PlayerHideON();
	UFUNCTION()
	void PlayerHideOFF();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="MyPlayerSettings")
	bool bIsHiding = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="MyPlayerSettings")
	bool bIsInHideZone = false;

	//훔치기
	UPROPERTY()
	class AMH_TargetItem* OverlappingItem;
	bool bCanPickup = false;

		
};
