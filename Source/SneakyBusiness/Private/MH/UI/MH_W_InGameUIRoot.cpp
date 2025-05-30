// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_InGameUIRoot.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "LHM/GameSystem/SBGameMode.h"
#include "LHM/UI/GameUI.h"
#include "LHM/UI/RankResultWidget.h"
#include "MH/MH_SBPlayerController.h"
#include "MH/Player_Nick.h"
#include "MH/UI/MH_W_Exit.h"
#include "MH/UI/MH_W_GameClear.h"
#include "MH/UI/MH_W_GameOver.h"
#include "MH/UI/MH_W_InGameMenu.h"
#include "MH/UI/MH_W_Options.h"

void UMH_W_InGameUIRoot::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (WBPInGameMenu)
	{
		WBPInGameMenu->OnInGameMenuExitPressed.AddDynamic(this, &UMH_W_InGameUIRoot::ShowInGameExit);
		WBPInGameMenu->OnInGameMenuResumePressed.AddDynamic(this, &UMH_W_InGameUIRoot::HandleBackPressed);
		WBPInGameMenu->OnInGameMenuOptionsPressed.AddDynamic(this, &UMH_W_InGameUIRoot::ShowInGameOptions);
		WBPInGameMenu->OnInGameMainMenuPressed.AddDynamic(this,&UMH_W_InGameUIRoot::OnInGameUIMainMenuClicked);
		WBPInGameMenu->OnInGameMainReStartPressed.AddDynamic(this, &UMH_W_InGameUIRoot::ReStartStage);
	}
	if (WBPInGameExit)
	{
		WBPInGameExit->OnStayPressed.AddDynamic(this, &UMH_W_InGameUIRoot::HandleBackPressed);
	}
	if (WBPInGameOptions)
	{
		WBPInGameOptions->OnBackPressed.AddDynamic(this, &UMH_W_InGameUIRoot::HandleBackPressed);
	}

	if (WBPGameClear)
	{
		WBPGameClear->OnNextStagePressed.AddDynamic(this,&UMH_W_InGameUIRoot::OnInGameUINextStageClicked);
		WBPGameClear->OnGameClearMainMenuPressed.AddDynamic(this,&UMH_W_InGameUIRoot::OnInGameUIMainMenuClicked);
	}
	if (WBPGameOver)
	{
		WBPGameOver->OnGameOverMainMenuPressed.AddDynamic(this,&UMH_W_InGameUIRoot::OnInGameUIMainMenuClicked);
		WBPGameOver->OnReStartStagePressed.AddDynamic(this,&UMH_W_InGameUIRoot::ReStartStage);
	}

	
	SetExitVisible(false);
	SetOptionsVisible(false);
	SetInGameMenuVisible(false);
}

void UMH_W_InGameUIRoot::SetExitVisible(bool bIsVisible)
{
	if (WBPInGameExit)
	{
		if (bIsVisible)
		{
			WBPInGameExit->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			WBPInGameExit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMH_W_InGameUIRoot::SetOptionsVisible(bool bIsVisible)
{
	if (WBPInGameOptions)
	{
		if (bIsVisible)
		{
			WBPInGameOptions->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			WBPInGameOptions->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMH_W_InGameUIRoot::ShowInGameExit()
{
	SetActiveMenu(EInGameUIPage::Exit);
}

void UMH_W_InGameUIRoot::ShowInGameOptions()
{
	SetActiveMenu(EInGameUIPage::Options);
}

void UMH_W_InGameUIRoot::OnInGameUIMainMenuClicked()
{
	UGameplayStatics::OpenLevel(this,FName("MH_LV_MainUI"));
}

void UMH_W_InGameUIRoot::OnInGameUINextStageClicked()
{
	//클리어 되는 순간 GI에 정보 보내야함.
	//수정필요
	//GI 정보 확인? 지금이 몇번 쨰 스테이지인지에 따라 다음 레벨 스테이지로 이동.
	UGameplayStatics::OpenLevel(this,FName("MH_LV_Stage02"));
}

void UMH_W_InGameUIRoot::ReStartStage()
{
	//수정필요
	//GI 정보 확인? 지금이 몇번 쨰 스테이지인지에 따라 해당 레벨 스테이지로 이동. 다시시작.
	UGameplayStatics::OpenLevel(this,FName("MH_LV_Stage01"));
}

void UMH_W_InGameUIRoot::ShowInGameClear()
{
	SetActiveMenu(EInGameUIPage::GameClear);
	
}

void UMH_W_InGameUIRoot::ShowInGameOver()
{
	SetActiveMenu(EInGameUIPage::GameOver);
}

void UMH_W_InGameUIRoot::SetActiveMenu(EInGameUIPage Page)
{
	CurrentPage = Page;

	switch (Page)
	{
	case EInGameUIPage::InGameHUD:
		WS_InGameUIRoot->SetActiveWidget(WBPInGameHUD);
		break;

	case EInGameUIPage::InGameMenu:
		SetInGameMenuVisible(true);
		break;

	case EInGameUIPage::Options:
		SetOptionsVisible(true);
		break;

	case EInGameUIPage::Exit:
		SetExitVisible(true);
		break;
		
	case EInGameUIPage::GameOver:
		WS_InGameUIRoot->SetActiveWidget(WBPGameOver);
		break;
		
	case EInGameUIPage::GameClear:
		WS_InGameUIRoot->SetActiveWidget(WBPGameClear);
		break;

	default:
		break;
	}
	
	
}

void UMH_W_InGameUIRoot::HandleBackPressed()
{
	if (CurrentPage == EInGameUIPage::Options)
	{
		SetOptionsVisible(false);
		SetActiveMenu(EInGameUIPage::InGameMenu);
	}
	else if (CurrentPage == EInGameUIPage::Exit)
	{
		SetExitVisible(false);
		SetActiveMenu(EInGameUIPage::InGameMenu);
	}
	else if (CurrentPage == EInGameUIPage::InGameMenu)
	{
		SetInGameMenuVisible(false);
		SetActiveMenu(EInGameUIPage::InGameHUD);
	}
}

void UMH_W_InGameUIRoot::ShowInGameMenu()
{
	if (CurrentPage == EInGameUIPage::InGameHUD)
	{
		SetActiveMenu(EInGameUIPage::InGameMenu);
	}
}

void UMH_W_InGameUIRoot::SetInGameMenuVisible(bool bIsVisible)
{
	if (WBPInGameMenu)
	{
		if (bIsVisible)
		{
			WBPInGameMenu->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			WBPInGameMenu->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
