// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MH_Lever.generated.h"

UCLASS()
class SNEAKYBUSINESS_API AMH_Lever : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMH_Lever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Alert")
	class UMH_EnemyAlertComp* EnemyAlertComp;
	
	UPROPERTY(EditAnywhere, Category="Alert")
	float AlertRadius = 600.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsLeverActive = false;
	
	UFUNCTION()
	void ActivateLever();

	UFUNCTION(BlueprintImplementableEvent, Category="Alert")
	void LeverAnim();

};
