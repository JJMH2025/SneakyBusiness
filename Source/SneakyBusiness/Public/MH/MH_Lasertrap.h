// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MH_Lasertrap.generated.h"

UCLASS()
class SNEAKYBUSINESS_API AMH_Lasertrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMH_Lasertrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//맞은 플레이어 주변 탐색 반경
	UPROPERTY(EditAnywhere)
	float DetectionRenge = 800.f;

	//방향을 바꾸며 발사되는 간격
	UPROPERTY(EditAnywhere)
	float LaserInterval = 1.5f;

	//최대 거리
	UPROPERTY(EditAnywhere)
	float LaserLength = 2000.f;

	//레이저 발사 방향
	UPROPERTY(EditAnywhere)
	TArray<FVector> Directions;

	//현재 레이저 방향 인덱스
	UPROPERTY()
	int32 CurrentDirIdx = 0;

	//발사 타이머 핸들
	FTimerHandle LaserTimerHandle;

	//플레이어 맞았는지 체크
	UFUNCTION()
	void FireLaser();
	
};
