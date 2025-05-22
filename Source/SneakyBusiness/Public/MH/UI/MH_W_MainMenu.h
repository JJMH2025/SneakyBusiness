// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_MainMenu.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOptionsPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitPressed);

UCLASS()
class SNEAKYBUSINESS_API UMH_W_MainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	FOnStartPressed	OnStartPressed;
	
	UPROPERTY(EditAnywhere)
	FOnOptionsPressed	OnOptionsPressed;

	UPROPERTY(EditAnywhere)
	FOnExitPressed	OnExitPressed;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Options;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnExitClicked();
	
};
