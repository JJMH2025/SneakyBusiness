// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_Start.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UMH_W_Start : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Tutorial;

	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Stage01;

	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Stage02;
	
	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Stage03;

	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Stage04;

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
};
