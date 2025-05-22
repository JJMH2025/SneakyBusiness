// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "RankResultWidget.generated.h"

USTRUCT(BlueprintType)
struct FRankEntry
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly)
	int32 Score;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Rank;
};

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API URankResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	//virtual void LoadObject() override;

public:
	UFUNCTION(BlueprintCallable)
	void InitRankResult(int32 StageIndex, int32 MyScore, int32 MyRank, const TArray<FRankEntry>& TopRanks);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URankEntryWidget> RankEntryWidgetClass;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Stage;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_MyScore;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_MyRank;

	UPROPERTY(meta = (BindWidget))
	class UListView* ListView_Rank;

	// ³»ºÎ Ä³½Ì¿ë
	UPROPERTY(BlueprintReadOnly)
	TArray<FRankEntry> RankEntries;
};
