// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_GameClear.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameClearMainMenuPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextStagePressed);

UCLASS()
class SNEAKYBUSINESS_API UMH_W_GameClear : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable)
	FOnGameClearMainMenuPressed OnGameClearMainMenuPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnNextStagePressed OnNextStagePressed;
	
	UPROPERTY(meta = (BindWidget))
	class URankResultWidget* WBPViewRankings;
		
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameClearMainMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_NextStage;
	
	UFUNCTION()
	void OnNextStageClicked();
	
	UFUNCTION()
	void OnGameClearMainMenuClicked();

	
};
