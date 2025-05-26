// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_GameOver.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverMainMenuPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReStartStagePressed);

UCLASS()
class SNEAKYBUSINESS_API UMH_W_GameOver : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable)
	FOnGameOverMainMenuPressed OnGameOverMainMenuPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnReStartStagePressed OnReStartStagePressed;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameOverReStartStage;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameOverMainMenu;

	UFUNCTION()
	void OnGameOverReStartStageClicked();

	UFUNCTION()
	void OnGameOverMainMenuClicked();
};
