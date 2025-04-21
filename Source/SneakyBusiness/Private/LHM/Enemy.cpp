#include "LHM/Enemy.h"
#include "MH/MH_ShootComp.h"
#include "MH/Player_Nick.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "LHM/AI/EnemyAIController.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

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
		FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
		FVector Forward = GetActorForwardVector();

		float SightRadius = SightConfig->SightRadius;
		float HalfFOVRadians = FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees / 2.0f);

		DrawDebugCone(GetWorld(), Start, Forward, SightRadius, HalfFOVRadians, HalfFOVRadians, 12, FColor::Green, false, 0.1f, 0, 2.0f);
	}

	// 순찰 중 회전 보간
	if (bIsRotating) LerpRotation(DeltaTime);
	else bMovingForward = GetActorForwardVector().X >= 0.1 ? true : false;

	// 추적 중 공간 이동 보간
	if (bIsMovingDepth) LerpMoveToDepth(DeltaTime);
}

void AEnemy::Patrol()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is patrolling."));

	// 회전 중이거나 기절 상태면 이동하지 않음
	if (bIsRotating) return;

	// 플레이어 감지
	if (IsPlayerDetectedByAIPerception())
	{
		SetEnemyAIState(EEnemyAIState::MovingToOtherSpace);
		return;
	}

	// 장애물 감지
	if (IsObstacleAhead(GetActorForwardVector(), 150.0f))
	{
		UE_LOG(LogTemp, Log, TEXT("In Patrol 장애물 감지"));
		// 방향 반전 + 회전
		float RotationAmount = bMovingForward ? -180.0f : 180.0f;
		TargetRot = GetActorRotation() + FRotator(0, RotationAmount, 0);
		bIsRotating = true;
	}

	// 정면으로 이동
	AddMovementInput(GetActorForwardVector(), 0.2f);
}

void AEnemy::AlignXToPlayer()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is Align X To Player."));

	APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player) return;

	if (Player->bIsPlayerLoc != bIsASpace)
	{
		// 먼저 X축 정렬을 시도
		FVector Dir = FVector((Player->GetActorLocation().X - GetActorLocation().X), 0, 0).GetSafeNormal();
		AddMovementInput(Dir, 0.5f);

		float XDiff = FMath::Abs(Player->GetActorLocation().X - GetActorLocation().X);
		if (XDiff < 100.0f)
		{
			SetEnemyAIState(EEnemyAIState::MovingToOtherSpace);
			return;
		}
	}
	else
	{
		SetEnemyAIState(EEnemyAIState::Chase);
		return;
	}
}

void AEnemy::PrepareMoveToOtherSpace()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is Prepare Move To OtherSpace"));

	if (!bIsMovingDepth)
	{
		float OffsetY = bIsASpace ? -90.0f : 90.0f; // A → B : -90 / B → A : +90
		MoveDepthLocation = GetActorLocation() + FVector(0, OffsetY, 0);

		FVector DirectionToDepth;

		// 에너미가 왼쪽을 보고 있으면 오른쪽 방향 감지
		if(GetActorForwardVector().X < 0)
		{ 
			DirectionToDepth = GetActorRightVector();
		}
		// 에너미가 오른쪽을 보고 있으면 왼쪽 방향 감지
		else
		{
			DirectionToDepth = -GetActorRightVector();
		}

		// B공간으로 이동할 때 장애물 있으면 다시 X축 정렬
		if (IsObstacleAhead(DirectionToDepth, 90))
		{
			UE_LOG(LogTemp, Log, TEXT("In Prepare Move To Other Space 장애물 감지"));
			APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (!Player) return;

			// X축 정렬 시도
			FVector Dir = FVector((Player->GetActorLocation().X - GetActorLocation().X), 0, 0).GetSafeNormal();
			AddMovementInput(Dir, 0.5f);

			float XDiff = FMath::Abs(Player->GetActorLocation().X - GetActorLocation().X);
			if (XDiff < 1.0f)
			{
				bIsMovingDepth = true;
				return;
			}
		}
		// 장애물 없으면 B공간으로 이동
		else
		{
			UE_LOG(LogTemp, Log, TEXT("In Prepare Move To Other Space 장애물 없음 바로 bIsMovingDepth 트루"));

			bIsMovingDepth = true;
			return;
		}
	}
}

void AEnemy::Chase()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is chasing the player!"));

	if (bIsRotating || IsPlayerStateToFrozenOrDead())
	{
		SetEnemyAIState(EEnemyAIState::Patrol);
		return;
	}

	// 플레이어 쫓다가 장애물 있으면 뒤돌아서 순찰 상태로 전환
	if (IsObstacleAhead(GetActorForwardVector(), 130.0f))
	{
		UE_LOG(LogTemp, Log, TEXT("In Chase 장애물 감지"));
		float RotationAmount = bMovingForward ? -180.0f : 180.0f;
		TargetRot = GetActorRotation() + FRotator(0, RotationAmount, 0);
		bIsRotating = true;

		SetEnemyAIState(EEnemyAIState::Patrol);
		return;
	}
	else
	{
		AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!Player) return;

		// 플레이어와의 거리 체크 (300 이하이면 Attack 상태로 전환)
		if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) <= 300.0f)
		{
			SetEnemyAIState(EEnemyAIState::Attack);
			return;
		}
		else
		{
			// 플레이어 방향으로 이동
			FVector Dir = FVector((Player->GetActorLocation().X - GetActorLocation().X), 0, 0).GetSafeNormal();
			AddMovementInput(Dir, 0.5f);
		}
	}
}

void AEnemy::Attack()
{
	//GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is attacking!"));
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));

	// 플레이어 감지 못하면 순찰상태로 전환
	if (!IsPlayerDetectedByAIPerception())
	{
		SetEnemyAIState(EEnemyAIState::Patrol);
		return;
	}

	if (!bAttackStarted)
	{
		bAttackStarted = true;
		// 1초 후 총알 발사
		GetWorldTimerManager().SetTimer(
			AttackTimerHandle, 
			this, 
			&AEnemy::DoShooting, 
			1.0f, 
			false
		);
	}
}

void AEnemy::Signal()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is signaling nearby enemies!"));
}

void AEnemy::HitByDoor()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy was hit by a door and is stunned!"));
	
	// 문에 부딪힌 애니메이션 재생

	// 5초 뒤에 깨어남
	GetWorldTimerManager().SetTimer(
		StunTimerHandle,
		this,
		&AEnemy::HandleHitByDoorAndStunEnd,
		5.0f,
		false
	);
}

void AEnemy::Stunned()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is stunned!"));

	//if (bIsStunned) return;
	//bIsStunned = true;

	// 기절하는 애니메이션 재생

	// 5초 뒤에 깨어남
	GetWorldTimerManager().SetTimer(
		StunTimerHandle,
		this,
		&AEnemy::HandleHitByDoorAndStunEnd,
		5.0f,
		false
	);
}

void AEnemy::WakeUp()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Enemy has woken up!"));
	//UE_LOG(LogTemp, Warning, TEXT("Enemy has woken up!"));

	// 일어나는 애니메이션 재생

	SetEnemyAIState(EEnemyAIState::Patrol);
}

void AEnemy::Alerted()
{
	if (bIsRotating) return;

	// 에너미의 방향에 따라 경보 위치로 이동하기 전 회전할지 여부
	// 에너미가 왼쪽을 바라보는지 판별
	bool bEnemyFacingLeft = GetActorForwardVector().X < 0;
	// 경보 위치가 에너미의 왼쪽에 있는지 판별
	bool bIsAlertLeftOfEnemy = AlertLocation.X < GetActorLocation().X;
	// 에너미가 바라보는 방향에 경보 위치가 없다면 회전해야 함
	bool bShouldTurn = bIsAlertLeftOfEnemy != bEnemyFacingLeft ? true : false;

	if (bShouldTurn)
	{
		float RotationAmount = bMovingForward ? -180.0f : 180.0f;
		TargetRot = GetActorRotation() + FRotator(0, RotationAmount, 0);
		bIsRotating = true;
	}

	// 목적지와 에너미의 거리 300 미만일 시 Patrol로 상태 전환
	float Dist = FVector::Dist(AlertLocation, GetActorLocation());
	if (Dist <= 200)
	{
		SetEnemyAIState(EEnemyAIState::Patrol);
	}
	
	if (!bIsRotating)
	{
		FVector Dir = FVector(AlertLocation.X - GetActorLocation().X, 0, 0).GetSafeNormal();
		AddMovementInput(Dir, 0.5);
	}
}

void AEnemy::ReactToTrapAlert(FVector InAlertLocation)
{
	UE_LOG(LogTemp, Log, TEXT("Called ReactToTrapAlert"));

	this->AlertLocation = InAlertLocation;
	SetEnemyAIState(EEnemyAIState::Alerted);
}

void AEnemy::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (bIsRotating) return;
	
	// 플레이어가 Frozen 또는 Dead 상태가 아니고
	// 플레이어가 숨은 상태인지 + 방향 비교로 감지 여부 판단
	if (IsPlayerStateToFrozenOrDead()) return;
	if (!ShouldDetectHiddenPlayer()) return;

	for (AActor* Actor : UpdatedActors)
	{
		if (APlayer_Nick* Player = Cast<APlayer_Nick>(Actor))
		{
			SetEnemyAIState(EEnemyAIState::MovingToAlignX);
			return;
		}
	}
}

void AEnemy::LerpRotation(float DeltaTime)
{
	FRotator CurrrentRot = GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrrentRot, TargetRot, DeltaTime, 5.0f);

	SetActorRotation(NewRot);

	// 목표 회전에 거의 도달하면 종료
	//if (FMath::Abs(NewRot.Yaw - TargetRot.Yaw) < 1.0f)
	float AngleDiff = FMath::FindDeltaAngleDegrees(NewRot.Yaw, TargetRot.Yaw);
	if (FMath::Abs(AngleDiff) < 1.0f)
	{
		SetActorRotation(TargetRot);
		bIsRotating = false;
	}
}

void AEnemy::LerpMoveToDepth(float DeltaTime)
{
	FVector NewLoc = FMath::Lerp(GetActorLocation(), MoveDepthLocation, 5.0f * DeltaTime);
	SetActorLocation(NewLoc);

	if (FVector::Dist(NewLoc, MoveDepthLocation) < 1.0f)
	{
		SetActorLocation(MoveDepthLocation);
		bIsASpace = !bIsASpace;
		bIsMovingDepth = false;
		SetEnemyAIState(EEnemyAIState::Chase);
	}
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

	bAttackStarted = false;
}

void AEnemy::HandleHitByDoorAndStunEnd()
{
	//bIsStunned = false;
	SetEnemyAIState(EEnemyAIState::WakeUp);
}

void AEnemy::ReceiveDamage()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Enemy is Received Damage!"));
	UE_LOG(LogTemp, Log, TEXT("Enemy is Received Damage!"));
	
	SetEnemyAIState(EEnemyAIState::Stunned);
}

bool AEnemy::IsPlayerDetectedByAIPerception()
{
	if (bIsRotating || bIsMovingDepth || bIsStunned) return false;

	// 플레이어가 Frozen 또는 Dead 상태가 아니고
	// 플레이어가 숨은 상태인지 + 방향 비교로 감지 여부 판단
	if(IsPlayerStateToFrozenOrDead()) return false;
	if(!ShouldDetectHiddenPlayer()) return false;

	TArray<AActor*> SensedActors;
	AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SensedActors);

	for (AActor* Actor : SensedActors)
	{
		if (APlayer_Nick* Player = Cast<APlayer_Nick>(Actor))
		{
			return true;
		}
	}
	return false;
}

bool AEnemy::IsObstacleAhead(FVector DirectionToDetect, float Distance)
{
	if(bIsRotating) return false;

	FVector Start = GetActorLocation() + FVector(0, 0, -30.0f);
	FVector End = Start + (DirectionToDetect * Distance);

	FHitResult HitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams, Params);
	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Blue, false, 1.0f, 0, 2.0f);

	return bHit;
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

bool AEnemy::ShouldDetectHiddenPlayer()
{
	APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(!Player) return false;

	// 플레이어가 숨지 않은 상태면 무조건 감지
	if(!Player->bIsHiding) return true;

	// 플레이어가 숨은 상태에서 방향 비교
	FVector EnemyForwardVec = GetActorForwardVector();

	// 에너미가 왼쪽을 바라보는지 판별
	bool bEnemyFacingLeft = EnemyForwardVec.X < 0;

	// 플레이어가 왼쪽을 보고 있다면 (LastHorizontalDirection == -1)
	bool bPlayerLookingLeft = Player->LastHorizontalDirection < 0;

	// 조건 : 에너미와 플레이어의 방향이 같아야 감지 가능
	return bEnemyFacingLeft == bPlayerLookingLeft;
}
