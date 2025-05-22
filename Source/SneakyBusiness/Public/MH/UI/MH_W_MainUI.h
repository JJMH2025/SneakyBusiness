// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_MainUI.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EUIPage : uint8
{
	Main,
	Start,
	Options,
	Exit
};

UCLASS()
class SNEAKYBUSINESS_API UMH_W_MainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UMH_W_MainMenu* WBPMainMenu;

	UPROPERTY(meta = (BindWidget))
	class UMH_W_Start* WBPStart;
	
	UPROPERTY(meta = (BindWidget))
	class UMH_W_Options* WBPOptions;
	
	UPROPERTY(meta = (BindWidget))
	class UMH_W_Exit* WBPExit;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WS_MainUI;

	UFUNCTION()
	void SetOptionsVisible(bool bIsVisible);

	UFUNCTION()
	void SetExitVisible(bool bIsVisible);

	UFUNCTION()
	void ShowStart();

	UFUNCTION()
	void ShowOptions();

	UFUNCTION()
	void ShowExit();
	
	UFUNCTION()
	void HandleBackPressed();
	
	UFUNCTION()
	void SetActiveMenu(EUIPage Page);

	UPROPERTY()
	EUIPage CurrentPage;
protected:
	
};