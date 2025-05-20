// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UMH_W_MainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Options;

	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Exit;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnExitClicked();
	
};
