#include "LHM/Enemy.h"
#include "LHM/FSMComponent.h"
#include "MH/MH_ShootComp.h"
#include "MH/Player_Nick.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Fsm = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComp"));
	ShootComp = CreateDefaultSubobject<UMH_ShootComp>(TEXT("ShootComp"));
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// 시야 설정
	SightConfig->SightRadius = 500.0f; // 플레이어를 감지할 수 있는 거리
	SightConfig->LoseSightRadius = 520.0f; // 플레이어를 시야에서 놓치는 거리 (약간 여유)
	SightConfig->PeripheralVisionAngleDegrees = 90.0f; // 양 옆으로 45도씩 총 90도 시야각

	// DetectionByAffiliation 어떤 팀 타입(Pawn) 감지할지 여부
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// 어떤 감지 센스를 사용할지 등록
	AIPerceptionComp->ConfigureSense(*SightConfig);
	// 이 AI가 사용하는 주 감지 타입 설정
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	// 델리게이트 바인딩 (감지 대상이 갱신될 때마다 호출)
	AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnPerceptionUpdated);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 시야 범위 시각화
	if (SightConfig)
	{
		FVector Start = GetActorLocation()+FVector(0,0,50.0f);
		FVector Forward = GetActorForwardVector();

		float SightRadius = SightConfig->SightRadius;
		float HalfFOVRadians = FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees / 2.0f);

		DrawDebugCone(GetWorld(), Start, Forward, SightRadius, HalfFOVRadians, HalfFOVRadians, 12, FColor::Green, false, 0.1f, 0, 2.0f);
	}

	// 순찰 중 회전 보간
	if (bIsRotating) LerpRotation(DeltaTime);

	// 이동 보간 적용
	if (bIsMovingSideways)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, SideTargetLocation, DeltaTime, 5.0f);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, SideTargetLocation) < 2.0f)
		{
			SetActorLocation(SideTargetLocation);
			bIsMovingSideways = false;
		}
	}
}

void AEnemy::Patrol()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is patrolling."));

	// 플레이어 감지
	if (!bIsRotating)
	{
		//if (IsPlayerDetected())
		if(IsPlayerDetectedByAIPerception())
		{
			Fsm->SetState(EEnemyState::Chase);
			return;
		}
	}

	// 회전 중이면 이동하지 않음
	if (bIsRotating) return;

	// 장애물 감지
	if (IsObstacleAhead(100.0f))
	{
		// 방향 반전 + 회전
		bMovingForward = !bMovingForward;
		float RotationAmount = bMovingForward ? -180.0f : 180.0f;
		TargetRot = GetActorRotation() + FRotator(0, RotationAmount, 0);
		bIsRotating = true;
		//bIsMoving = false;
		return;
	}

	// 정면으로 이동
	AddMovementInput(GetActorForwardVector(), Speed);
}

void AEnemy::Attack()
{
	GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is attacking!"));
	UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));

	// 플레이어 감지 못하면 순찰상태로 전환
	//if (!IsPlayerDetected())
	if(!IsPlayerDetectedByAIPerception())
	{
		Fsm->SetState(EEnemyState::Patrol);
		return;
	}

	if (!bAttackStarted)
	{
		bAttackStarted = true;
		// 1초 후 총알 발사
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::DoShooting, 1.0f, false);
	}
}

void AEnemy::Chase()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is chasing the player!"));

	if (IsPlayerStateToFrozenOrDead()) return;

	if (IsObstacleAhead(100.0f))
	{
		bMovingForward = !bMovingForward;
		float RotationAmount = bMovingForward ? -180.0f : 180.0f;
		TargetRot = GetActorRotation() + FRotator(0, RotationAmount, 0);
		bIsRotating = true;

		Fsm->SetState(EEnemyState::Patrol);
		return;
	}

	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(!Player) return;

	// 플레이어 방향으로 이동
	FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	AddMovementInput(Direction, 0.5f);

	// 플레이어와의 거리 체크 (300 이하이면 Attack 상태로 전환)
	if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) <= 300.0f)
	{
		Fsm->SetState(EEnemyState::Attack);
	}
}

void AEnemy::Signal()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is signaling nearby enemies!"));
}

void AEnemy::HitByDoor()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy was hit by a door and is stunned!"));
	Fsm->SetState(EEnemyState::Stun);
}

void AEnemy::Stun()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy is stunned!"));

	Hp = 1;
	// 3초 뒤 깨어남으로 상태 변경
	Fsm->SetState(EEnemyState::WakeUp);
}

void AEnemy::WakeUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy has woken up!"));
	Fsm->SetState(EEnemyState::Patrol);
}

void AEnemy::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (APlayer_Nick* Player = Cast<APlayer_Nick>(Actor))
		{
			if (!IsPlayerStateToFrozenOrDead())
			{
				Fsm->SetState(EEnemyState::Chase);
				return;
			}
		}
	}
}

bool AEnemy::IsPlayerDetectedByAIPerception()
{
	TArray<AActor*> SensedActors;
	AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SensedActors);

	for (AActor* Actor : SensedActors)
	{
		APlayer_Nick* Player = Cast<APlayer_Nick>(Actor);
		if (Player && !IsPlayerStateToFrozenOrDead())
		{
			return true;
		}
	}
	return false;
}

bool AEnemy::IsObstacleAhead(float Distance)
{
	FVector Start = GetActorLocation() + FVector(0, 0, -30.0f);
	FVector ForwardVec = GetActorForwardVector();
	FVector End = Start + (ForwardVec * Distance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, Params);
	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Blue, false, 1.0f, 0, 2.0f);

	return bHit;
}

//bool AEnemy::IsPlayerDetected()
//{
//	if(IsPlayerStateToFrozenOrDead()) return false;
//
//	FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
//
//	FVector ForwardVec = GetActorForwardVector();
//	FVector LeftVec = ForwardVec.RotateAngleAxis(-45, FVector(0, 0, 1));
//	FVector RightVec = ForwardVec.RotateAngleAxis(45, FVector(0, 0, 1));
//
//	FVector End = Start + (ForwardVec * 500.0f);
//	FVector EndLeft = Start + (LeftVec * 500.0f);
//	FVector EndRight = Start + (RightVec * 500.0f);
//
//	FHitResult HitResult, HitResultLeft, HitResultRight;
//	FCollisionQueryParams Params;
//	Params.AddIgnoredActor(this); // 에너미 제외
//
//	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params);
//	bool bHitLeft = GetWorld()->LineTraceSingleByChannel(HitResultLeft, Start, EndLeft, ECC_Pawn, Params);
//	bool bHitRight = GetWorld()->LineTraceSingleByChannel(HitResultRight, Start, EndRight, ECC_Pawn, Params);
//
//	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);
//	//DrawDebugLine(GetWorld(), Start, EndLeft, bHitLeft ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);
//	//DrawDebugLine(GetWorld(), Start, EndRight, bHitRight ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);
//
//	if (bHit)
//	{
//		AActor* HitActor = HitResult.GetActor();
//		if(HitActor && HitActor->ActorHasTag("Player"))
//		{
//			return true;
//		}
//	}
//
//	/*if (bHitLeft)
//	{
//		AActor* HitActor = HitResult.GetActor();
//		if (HitActor && HitActor->ActorHasTag("Player"))
//		{
//			MoveSideways(-100.0f);
//			return true;
//		}
//	}
//
//	if (bHitRight)
//	{
//		AActor* HitActor = HitResult.GetActor();
//		if (HitActor && HitActor->ActorHasTag("Player"))
//		{
//			MoveSideways(100.0f);
//			return true;
//		}
//	}*/
//
//	return false;
//}

void AEnemy::LerpRotation(float DeltaTime)
{
	FRotator CurrrentRot = GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrrentRot, TargetRot, DeltaTime, 5.0f);

	SetActorRotation(NewRot);

	// 목표 회전에 거의 도달하면 종료
	if (FMath::Abs(NewRot.Yaw - TargetRot.Yaw) < 1.0f)
	{
		SetActorRotation(TargetRot);
		bIsRotating = false;
	}
}

void AEnemy::MoveSideways(float OffsetY)
{
	// 현재 회전 값 저장
	float InitialYaw = GetActorRotation().Yaw;

	// 먼저 -90도 회전 시작
	TargetYaw = InitialYaw = 90.0f;

	// 이동 목표 설정
	SideTargetLocation = GetActorLocation() + FVector(0, OffsetY, 0);
	bIsMovingSideways = true;
}

void AEnemy::DoShooting()
{
	// 총알 발사
	if (ShootComp)
	{
		FVector FireLocation = GetActorLocation();
		FRotator FireRotation = GetActorRotation();
		ShootComp->Shooting(FireLocation, FireRotation);
	}

	// 공격 후 다시 Chase 상태로 전환해서 플레이어를 추적
	bAttackStarted = false;
	Fsm->SetState(EEnemyState::Patrol);
}

bool AEnemy::IsPlayerStateToFrozenOrDead()
{
	// MH요청 : 플레이어가 Frozen, Dead일 때 감지 못하게 설정
	APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		if (Player->CurrentPlayerState == EPlayerState::Frozen
		|| Player->CurrentPlayerState == EPlayerState::Invincible)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void AEnemy::ReceiveDamage()
{
	Hp -= 1;
	if (Hp <= 0) Fsm->SetState(EEnemyState::Stun);
}