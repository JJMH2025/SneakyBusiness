// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_InGameMenu.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInGameMenuResumePressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInGameMenuExitPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInGameMenuOptionsPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInGameMainMenuPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInGameMainReStartPressed);

UCLASS()
class SNEAKYBUSINESS_API UMH_W_InGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable)
	FOnInGameMenuResumePressed OnInGameMenuResumePressed;

	UPROPERTY(BlueprintAssignable)
	FOnInGameMenuExitPressed OnInGameMenuExitPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnInGameMenuExitPressed OnInGameMenuOptionsPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnInGameMainMenuPressed OnInGameMainMenuPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnInGameMainReStartPressed OnInGameMainReStartPressed;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Resume;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_ReStart;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_InGameMainMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Options;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;

	UFUNCTION()
	void OnResumeClicked();
	
	UFUNCTION()
	void OnInGameMainmenuClicked();

	UFUNCTION()
	void OnExitClicked();
	
	UFUNCTION()
	void OnOptionsClicked();
	
	UFUNCTION()
	void OnReStartClicked();

	
	
};
