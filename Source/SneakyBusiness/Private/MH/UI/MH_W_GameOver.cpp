// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_GameOver.h"

#include "Components/Button.h"

void UMH_W_GameOver::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_GameOverMainMenu->OnClicked.AddDynamic(this, &UMH_W_GameOver::OnGameOverMainMenuClicked);

	Btn_GameOverReStartStage->OnClicked.AddDynamic(this, &UMH_W_GameOver::OnGameOverReStartStageClicked);
}

void UMH_W_GameOver::OnGameOverReStartStageClicked()
{
	OnReStartStagePressed.Broadcast();
}

void UMH_W_GameOver::OnGameOverMainMenuClicked()
{
	OnGameOverMainMenuPressed.Broadcast();
}
