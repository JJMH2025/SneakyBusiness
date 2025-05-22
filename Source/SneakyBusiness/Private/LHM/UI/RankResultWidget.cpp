// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/UI/RankResultWidget.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "LHM/UI/RankEntryObject.h"
#include "LHM/UI/RankEntryWidget.h"

void URankResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RankEntryWidgetClass)
	{
		RankEntryWidgetClass = LoadClass<URankEntryWidget>(nullptr, TEXT("/Game/LHM/UI/WBP_RankEntry.WBP_RankEntry_C"));
	}
}

void URankResultWidget::InitRankResult(int32 StageIndex, int32 MyScore, int32 MyRank, const TArray<FRankEntry>& TopRanks)
{
	if (Txt_Stage && Txt_MyScore && Txt_MyRank && ListView_Rank)
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("stage: %d\nMyscore: %d\nMyRank: %d"), StageIndex, MyScore, MyRank));


		Txt_Stage->SetText(FText::FromString(FString::Printf(TEXT("Stage %d"), StageIndex)));
		Txt_MyScore->SetText(FText::AsNumber(MyScore));
		Txt_MyRank->SetText(FText::FromString(FString::Printf(TEXT("My Rank : %d"), MyRank)));
		ListView_Rank->ClearListItems();
		RankEntries = TopRanks;

		for (const FRankEntry& Entry : RankEntries)
		{
			URankEntryObject* EntryObj = NewObject<URankEntryObject>(this);
			EntryObj->SetPlayerName(Entry.PlayerName);
			EntryObj->SetScore(Entry.Score);
			EntryObj->SetRank(Entry.Rank);

			ListView_Rank->AddItem(EntryObj);
		}
	}
}
