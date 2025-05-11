// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/AI/BTS_Detect.h"
#include "LHM/Enemy/Enemy.h"
#include "LHM/AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MH/Player_Nick.h"

UBTS_Detect::UBTS_Detect()
{
	NodeName = TEXT("Detect Player");
	bNotifyBecomeRelevant = true;
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Enemy || !BB) return;

	// 시야 디버그 콘
	if (Enemy->SightConfig)
	{
		FVector Start = Enemy->GetActorLocation() + FVector(0, 0, 50.0f);
		FVector Forward = Enemy->GetActorForwardVector();

		float SightRadius = Enemy->SightConfig->SightRadius;
		float HalfFOVRadians = FMath::DegreesToRadians(Enemy->SightConfig->PeripheralVisionAngleDegrees / 2.0f);

		DrawDebugCone(GetWorld(), Start, Forward, SightRadius, HalfFOVRadians, HalfFOVRadians, 12, FColor::Green, false, 0.1f, 0, 2.0f);
	}

	// 감지 여부 확인
	TArray<AActor*> SensedActors;
	Enemy->AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SensedActors);
	
	for (AActor* Actor : SensedActors)
	{
		// 감지된 액터가 플레이어일 때
		if (APlayer_Nick* Player = Cast<APlayer_Nick>(Actor))
		{
			// 플레이어가 감지 가능한 상태인지 체크
			if (!Enemy->IsPlayerStateToFrozenOrDead() && Enemy->ShouldDetectHiddenPlayer())
			{
				DrawDebugLine(
					Enemy->GetWorld(),
					Enemy->GetActorLocation(),
					Player->GetActorLocation(),
					FColor::Red,
					false,
					0.2f
				);

				DrawDebugPoint(
					Enemy->GetWorld(),
					Player->GetActorLocation(),
					10.0f,
					FColor::Red,
					false,
					0.2f
				);

				// 블랙보드 값 갱신
				BB->SetValueAsObject(FName("Player"), Player);
				return;
			}
		}
	}
	
	// 감지 실패 시 플레이어 키 초기화
	BB->ClearValue(FName("Player"));
}
