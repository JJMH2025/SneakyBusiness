// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "RankEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API URankEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	/*UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_PlayerName;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Score;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Rank;*/

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_PlayerName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_Score;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_Rank;
};
