// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SBHUD.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API ASBHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ASBHUD();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameUI> GameUIClass;

	UPROPERTY()
	class UGameUI* GameUI;
};
