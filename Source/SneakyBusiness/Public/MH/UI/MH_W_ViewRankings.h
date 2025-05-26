// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MH_W_ViewRankings.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnViewRankingsBackPressed);
UCLASS()
class SNEAKYBUSINESS_API UMH_W_ViewRankings : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable)
	FOnViewRankingsBackPressed OnViewRankingsBackPressed;

	UPROPERTY(meta = (BindWidget))
	class URankResultWidget* WBPViewRankings;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_ViewRankingsBack;

	UFUNCTION()
	void OnViewRankingsBackClicked();
	
};
