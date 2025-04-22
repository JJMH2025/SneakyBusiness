// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MH_SlipTrap.generated.h"

UCLASS()
class SNEAKYBUSINESS_API AMH_SlipTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMH_SlipTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Alert")
	class UMH_EnemyAlertComp* EnemyAlertComp;

	UPROPERTY(EditAnywhere, Category="Alert")
	float AlertRadius =600.f;
	
	UFUNCTION()
	void SlipStart(AActor* Player);

	UFUNCTION()
	void ResetCooldown();

	UFUNCTION()
	void ActivateTrapEffect();

	UFUNCTION()
	bool CanTrigger()const;

	//날아가는 힘
	UPROPERTY(EditAnywhere)
	int32 ForceAmount = 600;

	//트랩 앞,뒤 Y포지션 위치 A라인(앞 :0) or B라인(뒤 : 100)
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Trigger")
	float TrapDepthPosition = 0.f;
	//한번 밟고나서 6초간은 다시 밟아도 발동 안되도록. 체크 변수
	UPROPERTY()
	bool bIsOnCooldown = false;

	FTimerHandle TrapCooldownHandle;
	

};
