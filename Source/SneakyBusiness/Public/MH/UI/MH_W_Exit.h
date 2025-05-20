// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_Exit.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UMH_W_Exit : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Exit;

	UPROPERTY(meta = (BindWidget), Category = "UI")
	class UButton* Btn_Stay;

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnStayClicked();
};
