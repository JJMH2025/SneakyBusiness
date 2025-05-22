// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_MainUI.h"

#include "NetworkMessage.h"
#include "Components/WidgetSwitcher.h"
#include "MH/UI/MH_W_Exit.h"
#include "MH/UI/MH_W_Options.h"
#include "MH/UI/MH_W_Start.h"
#include "MH/UI/MH_W_MainMenu.h"

void UMH_W_MainUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (WBPMainMenu)
	{
		WBPMainMenu->OnStartPressed.AddDynamic(this, &UMH_W_MainUI::ShowStart);
		WBPMainMenu->OnOptionsPressed.AddDynamic(this, &UMH_W_MainUI::ShowOptions);
		WBPMainMenu->OnExitPressed.AddDynamic(this, &UMH_W_MainUI::ShowExit);
	}
	if (WBPExit)
	{
		WBPExit->OnStayPressed.AddDynamic(this, &UMH_W_MainUI::HandleBackPressed);
	}
	if (WBPOptions)
	{
		WBPOptions->OnBackPressed.AddDynamic(this, &UMH_W_MainUI::HandleBackPressed);
	}
	if (WBPStart)
	{
		WBPStart->OnBackPressed.AddDynamic(this, &UMH_W_MainUI::HandleBackPressed);
	}

	SetOptionsVisible(false);
	SetExitVisible(false);
}

void UMH_W_MainUI::SetOptionsVisible(bool bIsVisible)
{
	if (WBPOptions)
	{
		if (bIsVisible)
		{
			WBPOptions->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			WBPOptions->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMH_W_MainUI::SetExitVisible(bool bIsVisible)
{
	if (WBPExit)
	{
		if (bIsVisible)
		{
			WBPExit->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			WBPExit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMH_W_MainUI::ShowStart()
{
	SetActiveMenu(EUIPage::Start);
}

void UMH_W_MainUI::ShowOptions()
{
	SetActiveMenu(EUIPage::Options);
}

void UMH_W_MainUI::ShowExit()
{
	SetActiveMenu(EUIPage::Exit);
}

void UMH_W_MainUI::HandleBackPressed()
{
	if (CurrentPage == EUIPage::Options)
	{
		SetOptionsVisible(false);
		SetActiveMenu(EUIPage::Main);
	}
	else if (CurrentPage == EUIPage::Exit)
	{
		SetExitVisible(false);
		SetActiveMenu(EUIPage::Main);
	}
	else if (CurrentPage == EUIPage::Start)
	{
		SetActiveMenu(EUIPage::Main);
	}
}

void UMH_W_MainUI::SetActiveMenu(EUIPage Page)
{
	CurrentPage = Page;
	
	switch (Page)
	{
	case EUIPage::Main:
		WS_MainUI->SetActiveWidget(WBPMainMenu);
		break;

	case EUIPage::Start:
		WS_MainUI->SetActiveWidget(WBPStart);
		break;;

	case EUIPage::Options:
		SetOptionsVisible(true);
		break;

	case EUIPage::Exit:
		SetExitVisible(true);
		break;


	default:
		break;
	}
}
