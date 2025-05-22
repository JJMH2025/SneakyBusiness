// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_Start.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartBackPressed);

UCLASS()
class SNEAKYBUSINESS_API UMH_W_Start : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	FOnStartBackPressed	OnBackPressed;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Tutorial;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Stage01;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Stage02;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Stage03;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Stage04;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Back;

	UFUNCTION()
	void OnTutorialClicked();

	UFUNCTION()
	void OnStage01Clicked();

	UFUNCTION()
	void OnStage02Clicked();
	
	UFUNCTION()
	void OnStage03Clicked();
	
	UFUNCTION()
	void OnStage04Clicked();
	
	UFUNCTION()
	void OnBackClicked();
};
