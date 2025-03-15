// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MH_ShootComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNEAKYBUSINESS_API UMH_ShootComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMH_ShootComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void AmmoSetUp();

	UFUNCTION()
	void Shooting(FVector FireLocation, FRotator FireRotation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	int32 MaxAmmo = 2;  // 최대 장전 가능 개수

	// 총알 개수 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	int32 CurrentAmmo = MaxAmmo;  // 기본 2발 장전 가능
	
	//총알 장전 함수
	UFUNCTION()
	void ReloadAmmo();
	//총 쏠수 있는지 여부
	bool bCanShoot = true;
	//쿨타임 타이머
	FTimerHandle ShootTimer;
	//발사쿨타임함수
	UFUNCTION()
	void ResetShoot();
	//발사간격, 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPlayerSettings")
	float ShootCooldown = 0.7f;
	
		
};
