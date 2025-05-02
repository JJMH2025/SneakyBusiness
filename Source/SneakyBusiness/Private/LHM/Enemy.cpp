#include "LHM/Enemy.h"
#include "MH/MH_ShootComp.h"
#include "MH/Player_Nick.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "LHM/AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	/*static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/LHM/BluePrints/AI/BB_Enemy.BB_Enemy"));
	if (BBAssetRef.Succeeded())
	{
		BBD = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/LHM/BluePrints/AI/BT_Enemy.BT_Enemy"));
	if (BTAssetRef.Succeeded())
	{
		BT = BTAssetRef.Object;
	}*/

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

void AEnemy::Signal()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is signaling nearby enemies!"));
}

void AEnemy::HitByDoor()
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (!AIController) return;
	
	AIController->SetBlackboardBoolValue("bHitByDoor", true);

	SetEnemyAIState(EEnemyAIState::Stunned);
}

void AEnemy::ReceiveDamage()
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (!AIController) return;
		
	AIController->SetBlackboardBoolValue("bTakeDamage", true);

	SetEnemyAIState(EEnemyAIState::Stunned);
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
			SetEnemyAIState(EEnemyAIState::Chase);
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

		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		if (!AIController) return;

		AIController->SetBlackboardBoolValue("bIsPlayerInOtherSpace", false);
	}
}

UMH_ShootComp& AEnemy::GetShootComp() const
{
	check(ShootComp);
	return *ShootComp;
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
