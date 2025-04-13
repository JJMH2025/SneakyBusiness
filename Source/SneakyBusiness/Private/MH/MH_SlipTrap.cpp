// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_SlipTrap.h"

#include "Components/BoxComponent.h"
#include "MH/Player_Nick.h"

// Sets default values
AMH_SlipTrap::AMH_SlipTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComp
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	
}

// Called when the game starts or when spawned
void AMH_SlipTrap::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->SetSimulatePhysics(true);
}

// Called every frame
void AMH_SlipTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMH_SlipTrap::SlipStart(AActor* Player)
{
	if (Player == nullptr || bIsOnCooldown)
	{
		GEngine->AddOnScreenDebugMessage(-9, 5.f, FColor::Green,TEXT("CoolTime"));
		return;
	}
	
	bIsOnCooldown = true;
	
	GEngine->AddOnScreenDebugMessage(-9, 5.f, FColor::Green,TEXT("SlipStart"));
	FVector LaunchDir = -Player->GetActorForwardVector();
	LaunchDir.Z += 3.f;
	LaunchDir.Normalize();
	LaunchDir.Y += 0.f;
	LaunchDir.Normalize();

	BoxComponent->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Ignore);
	BoxComponent->AddImpulse(LaunchDir*ForceAmount,NAME_None,true);

	GetWorld()->GetTimerManager().SetTimer(TrapCooldownHandle,this,&AMH_SlipTrap::ResetCooldown,6.0f,false);

	ActivateTrapEffect();
}

void AMH_SlipTrap::ResetCooldown()
{
	GEngine->AddOnScreenDebugMessage(-9, 5.f, FColor::Green,TEXT("ResetCooldown"));
	BoxComponent->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Block);
	bIsOnCooldown = false;
}

void AMH_SlipTrap::ActivateTrapEffect()
{
	//밟으면 소리나기
	//에너미 불러들이기
}

bool AMH_SlipTrap::CanTrigger()const
{
	return !bIsOnCooldown;
}

