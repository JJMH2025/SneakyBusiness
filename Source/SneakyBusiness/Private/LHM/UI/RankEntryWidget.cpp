// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/UI/RankEntryWidget.h"
#include "LHM/UI/RankEntryObject.h"
#include "Components/TextBlock.h"

void URankEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	URankEntryObject* Entry = Cast<URankEntryObject>(ListItemObject);
	if (Entry)
	{
		if (Txt_PlayerName && Txt_Score && Txt_Score)
		{
			Txt_PlayerName->SetText(FText::FromString(Entry->GetPlayerName()));
			Txt_Score->SetText(FText::AsNumber(Entry->GetScore()));
			Txt_Rank->SetText(FText::AsNumber(Entry->GetRank()));
		}
	}
}
