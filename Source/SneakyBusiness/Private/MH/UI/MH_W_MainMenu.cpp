// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_MainMenu.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMH_W_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Btn_Start)
	{
		Btn_Start->OnClicked.AddDynamic(this,&UMH_W_MainMenu::OnStartClicked);
	}

	if (Btn_Options)
	{
		Btn_Options->OnClicked.AddDynamic(this,&UMH_W_MainMenu::OnOptionsClicked);
	}

	if (Btn_Exit)
	{
		Btn_Exit->OnClicked.AddDynamic(this,&UMH_W_MainMenu::OnExitClicked);
	}
	
}

void UMH_W_MainMenu::OnStartClicked()
{
	OnStartPressed.Broadcast();
}

void UMH_W_MainMenu::OnOptionsClicked()
{
	OnOptionsPressed.Broadcast();
}

void UMH_W_MainMenu::OnExitClicked()
{
	OnExitPressed.Broadcast();
}
