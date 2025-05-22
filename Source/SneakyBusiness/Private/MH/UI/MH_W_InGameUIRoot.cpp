// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_InGameUIRoot.h"

#include "MH/UI/MH_W_Exit.h"
#include "MH/UI/MH_W_InGameMenu.h"
#include "MH/UI/MH_W_Options.h"

void UMH_W_InGameUIRoot::NativeConstruct()
{
	Super::NativeConstruct();

	if (WBPInGameMenu)
	{
		WBPInGameMenu->OnInGameMenuExitPressed.AddDynamic(this, &UMH_W_InGameUIRoot::ShowInGameExit);
		WBPInGameMenu->OnInGameMenuResumePressed.AddDynamic(this, &UMH_W_InGameUIRoot::HandleBackPressed);
	}
	if (WBPInGameExit)
	{
		WBPInGameExit->OnStayPressed.AddDynamic(this, &UMH_W_InGameUIRoot::HandleBackPressed);
	}
	if (WBPInGameOptions)
	{
		WBPInGameOptions->OnBackPressed.AddDynamic(this, &UMH_W_InGameUIRoot::HandleBackPressed);
	}
	
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
	SetExitVisible(true);
}

void UMH_W_InGameUIRoot::SetActiveMenu(EInGameUIPage Page)
{
	CurrentPage = Page;

	switch (Page)
	{
	case EInGameUIPage::InGameHUD:
		
		break;

	case EInGameUIPage::InGameMenu:
	
		break;;

	case EInGameUIPage::Options:
		SetOptionsVisible(true);
		break;

	case EInGameUIPage::Exit:
		SetExitVisible(true);
		
	case EInGameUIPage::GameOver:
		//SetExitVisible(true);
		
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
		SetActiveMenu(EInGameUIPage::InGameHUD);
	}
}
