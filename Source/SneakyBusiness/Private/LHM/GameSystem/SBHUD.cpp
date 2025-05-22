// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/GameSystem/SBHUD.h"
#include "LHM/UI/GameUI.h"
#include "LHM/GameSystem/SBGameInstance.h"

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

	FString CurrentLevel = GetWorld()->GetMapName();
	
	// 레벨 이름에 _ 접두사를 위한 처리
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	//if (CurrentLevel == "LV_Test")
	if (CurrentLevel != "MH_LV_MainUI")
	{
		if (GameUIClass)
		{
			GameUI = CreateWidget<UGameUI>(GetWorld(), GameUIClass);

			if (GameUI)
			{
				GameUI->AddToViewport();
			}
		}
	}
	else if (CurrentLevel == "LV_Rank") // HM: Rank test level
	{
		USBGameInstance* GI = GetGameInstance<USBGameInstance>();
		if (GI)
		{
			GI->ReadTopRankers(); // 랭킹 불러오기
		}
	}
}
