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
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponentL;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponentR;
	
	//Secne : 회전 애니메이션  root
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootScene;

	//메쉬
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorMesh;
	
	
	UFUNCTION(Blueprintable)
	void DoorOpen(const FVector& InstigatorForward,AActor* InteractingActor);

	UFUNCTION(Blueprintable)
	void DoorClosed();

	bool bIsRotatingDoor; // 회전
	float TargetYaw; // 목표 회전 값

	//문열림 때 기절
	UFUNCTION()
	void OnDoorBeginOverlapDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnDoorEndOverlapDoor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	AActor* LastActorOpened;

	//문열리는 방향 0 : R , 1 : L 
	UPROPERTY()
	bool bDoorOpenedR;
	UPROPERTY()
	TSet<AActor*> OverlappingActorsL;
	UPROPERTY()
	TSet<AActor*> OverlappingActorsR;
};
