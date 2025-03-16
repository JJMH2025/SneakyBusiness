// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MH_Bullet.generated.h"

UCLASS()
class SNEAKYBUSINESS_API AMH_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMH_Bullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//이동
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class UProjectileMovementComponent* MovementComp;
	//콜리전
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class USphereComponent* CollisionComp;
	//메쉬
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY()
	class APlayer_Nick* Player_Nick;

	//총알을 쏜 주인이 에너미인지 플레이어인지 체크
	UFUNCTION()
	void SetOwnerActor(AActor* NewOwner);

	UPROPERTY()
	AActor* OwnerActor;
};
