// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_ViewRankings.h"

#include "Components/Button.h"

void UMH_W_ViewRankings::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_ViewRankingsBack->OnClicked.AddDynamic(this,&UMH_W_ViewRankings::OnViewRankingsBackClicked);
}

void UMH_W_ViewRankings::OnViewRankingsBackClicked()
{
	OnViewRankingsBackPressed.Broadcast();
}
