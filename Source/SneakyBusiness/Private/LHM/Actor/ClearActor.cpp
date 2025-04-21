// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/Actor/ClearActor.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include <LHM/GameSystem/SBGameState.h>
#include "MH/Player_Nick.h"

// Sets default values
AClearActor::AClearActor()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComp"));
	RootComponent = Trigger;

	Trigger->SetCollisionProfileName(TEXT("ClearActor"));

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AClearActor::OnTriggerBeginOverlap);
}

void AClearActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ASBGameState* GS = GetWorld()->GetGameState<ASBGameState>();
	if(!Player || !GS) return;

	if (OtherActor == Player)
	{
		GS->bRechedGoal = true;
	}
}

