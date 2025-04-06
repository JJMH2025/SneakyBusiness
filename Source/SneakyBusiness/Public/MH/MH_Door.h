// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MH_Door.generated.h"

UCLASS()
class SNEAKYBUSINESS_API AMH_Door : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMH_Door();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//Box Collision
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
	
	//Secne : 회전 애니메이션  root
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootScene;

	//메쉬
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorMesh;
	
	
	UFUNCTION(Blueprintable)
	void DoorOpen(const float LastDirection);

	UFUNCTION(Blueprintable)
	void DoorClosed();

	bool bIsRotatingDoor; // 회전
	float TargetYaw; // 목표 회전 값

};
