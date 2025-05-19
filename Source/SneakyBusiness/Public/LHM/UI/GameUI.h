// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API UGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    void InitUI();

protected:
    UFUNCTION(BlueprintCallable)
    void UpdateUI();

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Txt_Timer;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Txt_StageNum;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Txt_Score;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* HorzBox_HPContainer;
    
    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* HorzBox_StolenItemContainer;

	UPROPERTY(EditAnywhere, Category = "UI")
	class UTexture2D* HPImage;

	UPROPERTY(EditAnywhere, Category = "UI")
	class UTexture2D* StolenItemImage;

    TArray<class UImage*> HPSlots;
    TArray<class UImage*> ItemSlots;
};
