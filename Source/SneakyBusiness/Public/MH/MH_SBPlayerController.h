// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/MH_W_MainMenu.h"
#include "MH_SBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API AMH_SBPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AMH_SBPlayerController();
	
	UPROPERTY(EditAnywhere,Category = UI)
	TSubclassOf<class UMH_W_MainUI> MainUIClass;
	
};
