// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_InGameUIRoot.h"

#include "Kismet/GameplayStatics.h"
#include "MH/MH_SBPlayerController.h"
#include "MH/Player_Nick.h"
#include "MH/UI/MH_W_Exit.h"
#include "MH/UI/MH_W_InGameMenu.h"
#include "MH/UI/MH_W_Options.h"

void UMH_W_InGameUIRoot::NativeConstruct()
{
	Super::NativeConstruct();
	if (AMH_SBPlayerController* PC = Cast<AMH_SBPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->OnIngameMenuPressed.AddDynamic(this, &UMH_W_InGameUIRoot::ShowInGameMenu);
	}
	
	if (WBPInGameMenu)
	{
		WBPInGameMenu->OnInGameMenuExitPressed.AddDynamic(this, &UMH_W_InGameUIRoot::ShowInGameExit);
		WBPInGameMenu->OnInGameMenuResumePressed.AddDynamic(this, &UMH_W_InGameUIRoot::HandleBackPressed);
		WBPInGameMenu->OnInGameMenuOptionsPressed.AddDynamic(this, &UMH_W_InGameUIRoot::ShowInGameOptions);
	}
	if (WBPInGameExit)
	{
		WBPInGameExit->OnStayPressed.AddDynamic(this, &UMH_W_InGameUIRoot::HandleBackPressed);
	}
	if (WBPInGameOptions)
	{
		WBPInGameOptions->OnBackPressed.AddDynamic(this, &UMH_W_InGameUIRoot::HandleBackPressed);
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

void UMH_W_InGameUIRoot::SetActiveMenu(EInGameUIPage Page)
{
	CurrentPage = Page;

	switch (Page)
	{
	case EInGameUIPage::InGameHUD:
		//SetInGameMenuVisible(true);
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
		//SetExitVisible(true);
		break;
		
	case EInGameUIPage::GameClear:
		//SetExitVisible(true);
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