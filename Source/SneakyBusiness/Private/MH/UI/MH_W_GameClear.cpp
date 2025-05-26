// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_GameClear.h"

#include "Components/Button.h"

void UMH_W_GameClear::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_GameClearMainMenu->OnClicked.AddDynamic(this,&UMH_W_GameClear::OnGameClearMainMenuClicked);
	Btn_NextStage->OnClicked.AddDynamic(this,&UMH_W_GameClear::OnNextStageClicked);
}

void UMH_W_GameClear::OnNextStageClicked()
{
	OnNextStagePressed.Broadcast();
}

void UMH_W_GameClear::OnGameClearMainMenuClicked()
{
	OnGameClearMainMenuPressed.Broadcast();
}
