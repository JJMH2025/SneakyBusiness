// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_InGameMenu.h"

#include "Components/Button.h"

void UMH_W_InGameMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Resume)
	{
		Btn_Resume->OnClicked.AddDynamic(this,&UMH_W_InGameMenu::OnResumeClicked);
	}

	if (Btn_Exit)
	{
		Btn_Exit->OnClicked.AddDynamic(this,&UMH_W_InGameMenu::OnExitClicked);
	}
	
	if (Btn_Options)
	{
		Btn_Options->OnClicked.AddDynamic(this,&UMH_W_InGameMenu::OnOptionsClicked);
	}
		
}

void UMH_W_InGameMenu::OnResumeClicked()
{
	OnInGameMenuResumePressed.Broadcast();
}

void UMH_W_InGameMenu::OnExitClicked()
{
	OnInGameMenuExitPressed.Broadcast();
}

void UMH_W_InGameMenu::OnOptionsClicked()
{
	OnInGameMenuOptionsPressed.Broadcast();
}
