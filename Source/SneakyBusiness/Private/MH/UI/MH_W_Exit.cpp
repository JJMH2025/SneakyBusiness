// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_Exit.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMH_W_Exit::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Exit)
	{
		Btn_Exit->OnClicked.AddDynamic(this, &UMH_W_Exit::OnExitClicked);
	}
	if (Btn_Stay)
	{
		Btn_Stay->OnClicked.AddDynamic(this, &UMH_W_Exit::OnStayClicked);
	}
}

void UMH_W_Exit::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void UMH_W_Exit::OnStayClicked()
{
	//창닫기
}
