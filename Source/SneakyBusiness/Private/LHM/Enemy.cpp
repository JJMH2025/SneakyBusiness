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

	CurrentState = EEnemyAIState::Patrol;
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

	// 추적 중 공간 이동 보간
	if (bIsMovingDepth) LerpMoveToDepth(DeltaTime);
}

void AEnemy::Patrol()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is patrolling."));

	// 회전 중이거나 기절 상태면 이동하지 않음
	if (bIsRotating || bIsMovingDepth || bIsStunned) return;

	// 플레이어 감지
	if (IsPlayerDetectedByAIPerception())
	{
		CurrentState = EEnemyAIState::MovingToAlignX;
		return;
	}

	// 장애물 감지
	if (IsObstacleAhead(150.0f))
	{
		// 방향 반전 + 회전
		bMovingForward = !bMovingForward;
		float RotationAmount = bMovingForward ? -180.0f : 180.0f;
		TargetRot = GetActorRotation() + FRotator(0, RotationAmount, 0);
		bIsRotating = true;
		return;
	}

	// 정면으로 이동
	AddMovementInput(GetActorForwardVector(), 0.2f);
}

void AEnemy::AlignXToPlayer()
{
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
			CurrentState = EEnemyAIState::MovingToOtherSpace;
		}
	}
	else
	{
		CurrentState = EEnemyAIState::Chase;
	}
}

void AEnemy::PrepareMoveToOtherSpace()
{
	if (!bIsMovingDepth)
	{
		float OffsetY = bIsASpace ? -90.0f : 90.0f; // A → B : -90 / B → A : +90
		MoveTargetLocation = GetActorLocation() + FVector(0, OffsetY, 0);
		
		// Tick에서 bIsMovingDepth가 true이면 LerpMoveToDepth()호출 -> 이동 시작 -> chase로 상태 전환
		bIsMovingDepth = true;
	}
}

void AEnemy::Chase()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is chasing the player!"));

	if (bIsRotating || bIsMovingDepth || bIsStunned || IsPlayerStateToFrozenOrDead())
	{
		CurrentState = EEnemyAIState::Patrol;
		return;
	}

	// 플레이어 쫓다가 장애물 있으면 뒤돌아서 순찰 상태로 전환
	if (IsObstacleAhead(130.0f))
	{
		bMovingForward = !bMovingForward;
		float RotationAmount = bMovingForward ? -180.0f : 180.0f;
		TargetRot = GetActorRotation() + FRotator(0, RotationAmount, 0);
		bIsRotating = true;

		CurrentState = EEnemyAIState::Patrol;
	}
	else
	{
		AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!Player) return;

		// 플레이어와의 거리 체크 (300 이하이면 Attack 상태로 전환)
		if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) <= 300.0f)
		{
			CurrentState = EEnemyAIState::Attack;
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
	if(bIsStunned) return;

	//GEngine->AddOnScreenDebugMessage(-2, 5.f, FColor::Green, TEXT("Enemy is attacking!"));
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is attacking!"));

	// 플레이어 감지 못하면 순찰상태로 전환
	if (!IsPlayerDetectedByAIPerception())
	{
		CurrentState = EEnemyAIState::Patrol;
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
	CurrentState = EEnemyAIState::Stunned;
}

void AEnemy::Stun()
{
	if(bIsStunned) return;

	//UE_LOG(LogTemp, Warning, TEXT("Enemy is stunned!"));

	bIsStunned = true;
	Hp = 1;

	// 5초 뒤에 깨어남
	GetWorldTimerManager().SetTimer(
		StunTimerHandle,
		this,
		&AEnemy::HandleStunEnd,
		5.0f,
		false
	);
}

void AEnemy::WakeUp()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Enemy has woken up!"));
	//UE_LOG(LogTemp, Warning, TEXT("Enemy has woken up!"));

	CurrentState = EEnemyAIState::Patrol;
}

void AEnemy::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (bIsRotating || bIsStunned) return;
	
	for (AActor* Actor : UpdatedActors)
	{
		if (APlayer_Nick* Player = Cast<APlayer_Nick>(Actor))
		{
			if (!IsPlayerStateToFrozenOrDead())
			{
				AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
				if (AIController)
				{
					AIController->SetTargetActor(Player);
					CurrentState = EEnemyAIState::MovingToAlignX;
					return;
				}
			}
		}
	}
}

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

void AEnemy::LerpMoveToDepth(float DeltaTime)
{
	FVector NewLoc = FMath::Lerp(GetActorLocation(), MoveTargetLocation, 5.0f * DeltaTime);
	SetActorLocation(NewLoc);

	if (FVector::Dist(NewLoc, MoveTargetLocation) < 1.0f)
	{
		SetActorLocation(MoveTargetLocation);
		bIsASpace = !bIsASpace;
		bIsMovingDepth = false;
		CurrentState = EEnemyAIState::Patrol;
	}
}

void AEnemy::DoShooting()
{
	if(bIsStunned) return;
	
	// 총알 발사
	if (ShootComp)
	{
		FVector FireLocation = GetActorLocation();
		FRotator FireRotation = GetActorRotation();
		ShootComp->Shooting(FireLocation, FireRotation);
	}

	// 공격 후 다시 Chase 상태로 전환해서 플레이어를 추적
	bAttackStarted = false;
	CurrentState = EEnemyAIState::Patrol;
}

void AEnemy::HandleStunEnd()
{
	bIsStunned = false;
	CurrentState = EEnemyAIState::WakeUp;
}

void AEnemy::ReceiveDamage()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Enemy is Received Damage!"));
	UE_LOG(LogTemp, Log, TEXT("Enemy is Received Damage!"));
	Hp -= 1;
	if (Hp <= 0)
	{
		CurrentState = EEnemyAIState::Stunned;
	}
}

bool AEnemy::IsPlayerDetectedByAIPerception()
{
	if (bIsRotating || bIsMovingDepth || bIsStunned) return false;

	TArray<AActor*> SensedActors;
	AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SensedActors);

	for (AActor* Actor : SensedActors)
	{
		APlayer_Nick* Player = Cast<APlayer_Nick>(Actor);

		if (Player && !IsPlayerStateToFrozenOrDead())
		{
			// 플레이어가 숨은 상태인지 + 방향 비교로 감지 여부 판단
			if (ShouldDetectHiddenPlayer())
			{
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("플레이어 숨음 -> 감지 불가"));
				return false;
			}
		}
	}
	return false;
}

bool AEnemy::IsObstacleAhead(float Distance)
{
	if(bIsRotating) return false;

	FVector Start = GetActorLocation() + FVector(0, 0, -30.0f);
	FVector ForwardVec = GetActorForwardVector();
	FVector End = Start + (ForwardVec * Distance);

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
