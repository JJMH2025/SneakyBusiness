// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_InGameUIRoot.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EInGameUIPage : uint8
{
	InGameHUD,
	InGameMenu,
	Options,
	Exit,
	GameOver,
	GameClear
};

UCLASS()
class SNEAKYBUSINESS_API UMH_W_InGameUIRoot : public UUserWidget
{
	GENERATED_BODY()

	public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UGameUI* WBPInGameHUD;

	UPROPERTY(meta = (BindWidget))
	class UMH_W_InGameMenu* WBPInGameMenu;

	UPROPERTY(meta = (BindWidget))
	class UMH_W_Exit* WBPInGameExit;
	
	UPROPERTY(meta = (BindWidget))
	class UMH_W_Options* WBPInGameOptions;

	UFUNCTION()
	void SetActiveMenu(EInGameUIPage Page);
	
	UFUNCTION()
	void HandleBackPressed();

	UPROPERTY()
	EInGameUIPage CurrentPage;
	
	UFUNCTION()
	void SetExitVisible(bool bIsVisible);

	UFUNCTION()
	void SetOptionsVisible(bool bIsVisible);

	UFUNCTION()
	void ShowInGameExit();
	
};