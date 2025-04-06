// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/Player_Nick.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MaterialHLSLTree.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MH/MH_Door.h"

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

	//Comp
	ShootComp = CreateDefaultSubobject<UMH_ShootComp>(TEXT("ShootComp"));

	TargetYawLot = GetActorLocation().Y; // 초기 위치 설정
	bIsMovingDepth = false; // 초기 이동 상태 false

	ShootArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ShootArrowComp->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = false; // 이동 방향에 따라 회전
	bUseControllerRotationYaw = false;

	TargetYaw = 0.0f;
	LastHorizontalDirection = 1.0f; // 기본적으로 오른쪽을 바라보도록 설정
	//시작할 때엔 항상 플레이어 위치 앞공간.
	bIsPlayerLoc = true;
	bIsHiding = false;
}

// Called when the game starts or when spawned
void APlayer_Nick::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayer_Nick::OnPlayerBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayer_Nick::OnPlayerEndOverlap);

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
		NewLocation.Y = FMath::FInterpTo(NewLocation.Y, TargetYawLot, DeltaTime, 15.0f);
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
			bIsMovingDepth = false;
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
		float InterSpeed = 15.0f;

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
		EnhancedInputComp->BindAction(IAMoveHorizontal, ETriggerEvent::Completed, this, &APlayer_Nick::StopHorizontal);
		EnhancedInputComp->BindAction(IAMoveDepth, ETriggerEvent::Triggered, this, &APlayer_Nick::MoveDepth);
		EnhancedInputComp->BindAction(IAJump, ETriggerEvent::Triggered, this, &APlayer_Nick::JumpNick);
		EnhancedInputComp->BindAction(IAShoot, ETriggerEvent::Triggered, this, &APlayer_Nick::Shooting);
		EnhancedInputComp->BindAction(IATest1, ETriggerEvent::Triggered, this, &APlayer_Nick::TestF);
		EnhancedInputComp->BindAction(IAInteract, ETriggerEvent::Triggered, this, &APlayer_Nick::PlayerInteract);
	}
}

void APlayer_Nick::TestF()
{
	PlayerTakeDamage();
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

		//벽이 가까이 있다면 숨기 On
		if (!bIsHiding && bIsInHideZone)
		{
			PlayerHideON();
		}
	}
}

void APlayer_Nick::StopHorizontal(const FInputActionValue& Value)
{
	if (bIsHiding)
	{
		PlayerHideOFF();
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
	Jump();
}

void APlayer_Nick::PlayerInteract()
{
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green,TEXT("Push E"));
	if (bIsOverlapDoor)
	{
		if (OverlapDoor)
		{
			OverlapDoor->DoorOpen(LastHorizontalDirection);
		}
	}
}

void APlayer_Nick::RespawnSetup()
{
	CurrentPlayerHP = MaxHP;

	//숨어 있다가 죽었다면 
	bIsHiding = false;
	bIsInHideZone = false;

	//혹시 기절했거나 사망했다가 살아나면 
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		// 입력 다시 활성화
		EnableInput(PC);
	}
}

void APlayer_Nick::Shooting()
{
	//앞뒤 이동하고 있을 때, 쿨타임안됐을 때 , 총알 없을 때 발사 불가능.
	//방향바꾸는 회전하는 중간에 발사하면 하면 다른 방향으로 남아감
	//플레이어가 좌우 이동 할 때에는 직전 방향으로 발사하도록..
	//발사 방향을 0또는 180도 로만 하도록

	if (bIsMovingDepth)
	{
		GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green,TEXT("Rotating"));
		return;
	}
	if (ShootComp != nullptr)
	{
		//소켓 위치로 변경 예정
		FVector testLoc = ShootArrowComp->GetComponentLocation();
		FRotator testRot = ShootArrowComp->GetComponentRotation();
		ShootComp->Shooting(testLoc, testRot);
	}
}


void APlayer_Nick::PlayerTakeDamage()
{
	//한대 맞으면 , 무적상태 또는 기절이 아닐때에만 
	if (CurrentPlayerState == EPlayerState::Invincible || CurrentPlayerState == EPlayerState::Frozen)
		return;

	//HP -1
	CurrentPlayerHP -= 1;

	//HP 0되면 사망
	GEngine->AddOnScreenDebugMessage(-5, 5.f, FColor::Blue, FString::Printf(TEXT("%d"), CurrentPlayerHP));
	if (CurrentPlayerHP <= 0)
	{
		PlayerDie();
		return;
	}

	//기절
	StartFrozen();
	//기절 애니메이션

	//아이템 드롭
	DropItems();

	// 3초 후 무적 상태 진입
	GetWorldTimerManager().SetTimer(FrozenTimerHandle, this, &APlayer_Nick::StartInvincible, 3.0f, false);
}

void APlayer_Nick::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Wall"))
	{
		GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green,TEXT("BeginOverlap Wall"));
		bIsInHideZone = true;
	}

	if (OtherActor && OtherActor->ActorHasTag("Door"))
	{
		OverlapDoor = Cast<AMH_Door>(OtherActor);
		if (OverlapDoor)
		{
			GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green,TEXT("BeginOverlap Door"));
			bIsOverlapDoor = true;
		}
	}

	if (OtherActor && OtherActor->ActorHasTag("TargetItem"))
	{
		
	}
}

void APlayer_Nick::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Wall"))
	{
		GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green,TEXT("EndOverlap Wall"));
		if (bIsHiding)
		{
			bIsInHideZone = false;
			PlayerHideOFF();
		}
	}

	if (OtherActor && OtherActor->ActorHasTag("Door"))
	{
		if (OverlapDoor)
		{
			GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green,TEXT("EndOverlap Door"));
			bIsOverlapDoor = false;
			OverlapDoor->DoorClosed();
			OverlapDoor=nullptr;
		}
	}
}

void APlayer_Nick::StartFrozen()
{
	CurrentPlayerState = EPlayerState::Frozen;
	//움직임 X
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		GEngine->AddOnScreenDebugMessage(-7, 5.f, FColor::Green,TEXT("DisableInput"));
		// 입력 비활성화
		DisableInput(PC);
	}

	FString EnumName = StaticEnum<EPlayerState>()->GetNameStringByValue((int64)CurrentPlayerState);
	GEngine->AddOnScreenDebugMessage(-6, 5.f, FColor::Yellow, EnumName);
}

void APlayer_Nick::StartInvincible()
{
	CurrentPlayerState = EPlayerState::Invincible;


	GEngine->AddOnScreenDebugMessage(-7, 5.f, FColor::Green,TEXT("EnableInput"));
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		// 입력 다시 활성화 (무적 상태 때)
		EnableInput(PC);
	}


	//무적상태 애니메이션
	FString EnumName = StaticEnum<EPlayerState>()->GetNameStringByValue((int64)CurrentPlayerState);
	GEngine->AddOnScreenDebugMessage(-6, 5.f, FColor::Yellow, EnumName);
	// 3초 후 다시 Normal 상태로 복귀
	GetWorldTimerManager().SetTimer(InvincibleTimerHandle, this, &APlayer_Nick::ResetToNormal, 3.0f, false);
}

void APlayer_Nick::ResetToNormal()
{
	CurrentPlayerState = EPlayerState::Normal;
	FString EnumName = StaticEnum<EPlayerState>()->GetNameStringByValue((int64)CurrentPlayerState);
	GEngine->AddOnScreenDebugMessage(-6, 5.f, FColor::Yellow, EnumName);
	//애니메이션 효과 제거
}

void APlayer_Nick::DropItems()
{
	//현재훔친 물건이 있다면
	//모두 플레이어 발밑으로 떨구기
}

void APlayer_Nick::PlayerDie()
{
	CurrentPlayerState = EPlayerState::Dead;
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green,TEXT("Dieeeeeeee"));

	//움직임 X
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		GEngine->AddOnScreenDebugMessage(-7, 5.f, FColor::Green,TEXT("DisableInput"));
		// 입력 비활성화
		DisableInput(PC);
	}
}

void APlayer_Nick::PlayerHideON()
{
	//무적상태 또는 기절이 아닐때에만 
	if (CurrentPlayerState == EPlayerState::Invincible || CurrentPlayerState == EPlayerState::Frozen)
		return;

	bIsHiding = true;
	//숨기 애니메이션 Play
	GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Purple,TEXT("PlayerHideON"));


	//LastHorizontalDirection
	//왼쪽키 누르고 있으면 왼쪽으로
	//오른쪽키 누르고 있으면 오른쪽으로 
}

void APlayer_Nick::PlayerHideOFF()
{
	//숨기 체크 해지
	bIsHiding = false;

	//숨기 애니메이션 Stop
	GEngine->AddOnScreenDebugMessage(-3, 5.f, FColor::Purple,TEXT("PlayerHideOFF"));
}
