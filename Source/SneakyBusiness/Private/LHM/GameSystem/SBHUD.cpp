// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/GameSystem/SBHUD.h"
#include "LHM/UI/GameUI.h"

ASBHUD::ASBHUD()
{
	static ConstructorHelpers::FClassFinder<UGameUI> WBPGameUI(TEXT("/Game/LHM/UI/WBP_GameUI.WBP_GameUI_C"));
	if (WBPGameUI.Succeeded())
	{
		GameUIClass = WBPGameUI.Class;
	}
}

void ASBHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GameUIClass)
	{
		GameUI = CreateWidget<UGameUI>(GetWorld(), GameUIClass);

		if (GameUI)
		{
			GameUI->AddToViewport();
		}
	}
}
