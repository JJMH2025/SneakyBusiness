// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/Player_Nick.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MaterialHLSLTree.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayer_Nick::APlayer_Nick()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//카메라
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 0));
	SpringArmComp->SetRelativeRotation(FRotator(0, -90, 0));
	SpringArmComp->TargetArmLength = 560;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);

	TargetYawLot = GetActorLocation().Y; // 초기 위치 설정
	bIsMovingDepth = false; // 초기 이동 상태 false

	GetCharacterMovement()->bOrientRotationToMovement = false; // 이동 방향에 따라 회전
	bUseControllerRotationYaw = false;

	TargetYaw = 0.0f;
	LastHorizontalDirection = 1.0f; // 기본적으로 오른쪽을 바라보도록 설정
	//시작할 때엔 항상 플레이어 위치 앞공간.
	bIsPlayerLoc = true;
}

// Called when the game starts or when spawned
void APlayer_Nick::BeginPlay()
{
	Super::BeginPlay();

	//Enhanced Input Subsystem 설정
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PC->GetLocalPlayer());
		if (Subsystem && MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

// Called every frame
void APlayer_Nick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//앞뒤 이동처리
	if (bIsMovingDepth)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Y = FMath::FInterpTo(NewLocation.Y, TargetYawLot, DeltaTime, 7.0f);
		SetActorLocation(NewLocation);
		bIsRotating = true;

		if (FMath::Abs(NewLocation.Y - TargetYawLot) < 2.0f)
		{
			NewLocation.Y = TargetYawLot; // 정확히 목표 위치로 설정
			bIsMovingDepth = false; // 이동 중 상태 종료

			//플레이어 현재 위치가 앞(A)공간이라면 True, 뒤(B)라면 false
			bIsPlayerLoc = TargetYawLot == ALoc;

			//도달 한 뒤 직전 바라보던 방향으로 회전 해야하는데
			//만약 앞에서 오른쪽을 보고있다가 0 
			if (LastHorizontalDirection == 1)
			{
				TargetYaw = 0.f;
			}
			//왼쪽 보고있다가  180
			else if (LastHorizontalDirection == -1)
			{
				TargetYaw = 180.f;
			}
		}
	}

	//앞뒤이동 회전 처리
	if (bIsRotating && bIsMovingDepth)
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
		//현재 회전값
		// 현재 회전값 가져오기
		float CurrentYaw = GetActorRotation().Yaw;

		// 최단 거리 회전각 계산
		float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);

		//회전보간
		float InterSpeed = 15.f;

		float NewYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + DeltaYaw, GetWorld()->GetDeltaSeconds(), InterSpeed);
		//회전적용
		SetActorRotation(FRotator(0.f, NewYaw, 0.f));

		if (FMath::Abs(NewYaw - TargetYaw) < 1.0f)
		{
			NewYaw = TargetYaw;
			bIsRotating = false;
		}
	}
	//좌우이동 회전 처리
	else if (bIsRotating)
	{
		//현재 회전값
		// 현재 회전값 가져오기
		float CurrentYaw = GetActorRotation().Yaw;

		// 최단 거리 회전각 계산
		float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);

		//회전보간
		float InterSpeed = 7.0f;

		float NewYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + DeltaYaw, GetWorld()->GetDeltaSeconds(), InterSpeed);
		//회전적용
		SetActorRotation(FRotator(0.f, NewYaw, 0.f));
		//SetActorRelativeRotation(FRotator(0.f, NewYaw, 0.f));

		if (FMath::Abs(NewYaw - TargetYaw) < 1.0f)
		{
			bIsRotating = false;
		}
	}
}

// Called to bind functionality to input
void APlayer_Nick::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComp)
	{
		EnhancedInputComp->BindAction(IAMoveHorizontal, ETriggerEvent::Triggered, this, &APlayer_Nick::MoveHorizontal);
		EnhancedInputComp->BindAction(IAMoveDepth, ETriggerEvent::Triggered, this, &APlayer_Nick::MoveDepth);
	}
}

//좌우이동
void APlayer_Nick::MoveHorizontal(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	if (MoveValue != 0)
	{
		//이동 적용
		AddMovementInput(FVector(MoveValue, 0, 0));

		//마지막 좌우 이동 방향 저장
		LastHorizontalDirection = (MoveValue > 0) ? 1.0f : -1.0f;
		// 목표 회전값 설정
		TargetYaw = (MoveValue > 0) ? 0.0f : 180.0f;

		bIsRotating = true;
	}
}

//앞뒤이동
void APlayer_Nick::MoveDepth(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	if (MoveValue == 0) return;

	// 목표 위치를 설정하고 이동 시작
	if (MoveValue > 0 && FMath::Abs(GetActorLocation().Y - BLoc) > 2.0f)
	{
		//앞(A)공간 -> 뒤(B) 공간
		TargetYawLot = BLoc; //W를 눌렀을 때 뒤공간으로 이동
		bIsMovingDepth = true;
		if (bIsMovingDepth)
		{
			TargetYaw = -90.f;
		}
	}
	else if (MoveValue < 0 && FMath::Abs(GetActorLocation().Y - ALoc) > 2.0f)
	{
		//뒤(B) 공간 -> 앞(A)공간
		TargetYawLot = ALoc; //W를 눌렀을 때 뒤공간으로 이동
		bIsMovingDepth = true;
		
		if (bIsMovingDepth)
		{
			TargetYaw = 90.f;
		}
	}
}


void APlayer_Nick::JumpNick()
{
	//점프 높이 큐브에 맞추기
	//1.
}

void APlayer_Nick::Shoot()
{
	//총알 2개 연속 발사 가능.
	//2발 모두 발사시 총알 하나가 없어져야 새로운 총알 하나 발사가능
}
