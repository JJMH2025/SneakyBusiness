// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_Options.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOptionBackPressed);

UCLASS()
class SNEAKYBUSINESS_API UMH_W_Options : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	FOnOptionBackPressed	OnBackPressed;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Save;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Back;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Credits;
	
	UPROPERTY(meta = (BindWidget))
	class USlider* Sli_BGM;
	
	UPROPERTY(meta = (BindWidget))
	class USlider* Sli_SFX;

	UFUNCTION()
	void OnSaveClicked();
	
	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnCreditsClicked();
	
	UFUNCTION()
	void OnBGMVolumeChanged(float Value);


	UFUNCTION()
	void OnSFXVolumeChanged(float Value);

	//UPROPERTY(meta = (BindWidget))
	//class UComboBoxString* LanguageComboBox;

	//FUNCTION()
	//void OnLanguageChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	
};
