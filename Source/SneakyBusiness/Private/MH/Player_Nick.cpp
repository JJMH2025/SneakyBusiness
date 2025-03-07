// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/Player_Nick.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
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

	TargetY = GetActorLocation().Y; // 초기 위치 설정
	bIsMovingDepth = false; // 초기 이동 상태 false
}

// Called when the game starts or when spawned
void APlayer_Nick::BeginPlay()
{
	Super::BeginPlay();

	//Enhanced Input Subsystem 설정
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem&&MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext,0);
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
		NewLocation.Y = FMath::FInterpTo(NewLocation.Y, TargetY, DeltaTime, 5.0f);
		SetActorLocation(NewLocation);

		// 목표 위치에 도달하면 멈추기
		if (FMath::Abs(NewLocation.Y - TargetY) < 1.0f)
		{
			NewLocation.Y = TargetY; // 정확히 목표 위치로 설정
			bIsMovingDepth = false;  // 이동 중 상태 종료
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
		EnhancedInputComp->BindAction(IAMoveHorizontal,ETriggerEvent::Triggered,this,&APlayer_Nick::MoveHorizontal);
		EnhancedInputComp->BindAction(IAMoveDepth,ETriggerEvent::Triggered,this,&APlayer_Nick::MoveDepth);
	}
}
//좌우이동
void APlayer_Nick::MoveHorizontal(const FInputActionValue& Value)
{
	float MoveValue= Value.Get<float>();
	if (MoveValue != 0)
	{
		AddMovementInput(FVector(MoveValue,0, 0));
	}

	//좌 우 이동시 플레이어 이동하는 방향으로 회전
}
//앞뒤이동
void APlayer_Nick::MoveDepth(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	if (MoveValue == 0) return;

	// 목표 위치를 설정하고 이동 시작
	if (MoveValue > 0 && GetActorLocation().Y != BLoc)
	{
		TargetY = BLoc; // W를 눌렀을 때 뒤공간으로 이동
		bIsMovingDepth = true;
	}
	else if (MoveValue < 0 && GetActorLocation().Y != ALoc)
	{
		TargetY = ALoc; // S를 눌렀을 때 앞공간으로 이동
		bIsMovingDepth = true;
	}
	//앞,뒤 이동시 플레이어의 앞방향이 키 누른 방향으로 회전하도록
}


void APlayer_Nick::JumpNick()
{
	//점프 높이 큐브에 맞추기
}

void APlayer_Nick::Shoot()
{
	//총알 2개 연속 발사 가능.
	//2발 모두 발사시 총알 하나가 없어져야 새로운 총알 하나 발사가능
}

