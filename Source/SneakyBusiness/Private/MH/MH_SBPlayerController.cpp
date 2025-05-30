// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/MH_SBPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "LHM/GameSystem/SBGameMode.h"
#include "MH/UI/MH_W_InGameUIRoot.h"
#include "MH/UI/MH_W_MainUI.h"
#include "Settings/EditorStyleSettings.h"


AMH_SBPlayerController::AMH_SBPlayerController()
{
	
}

void AMH_SBPlayerController::GameOver()
{
	if (InGameUIRoot)
	{
		InGameUIRoot->ShowInGameOver();
	}
}

void AMH_SBPlayerController::GameClear()
{
	if (InGameUIRoot)
	{
		InGameUIRoot->ShowInGameClear();
	}
}

void AMH_SBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FString CurrentLevel = GetWorld()->GetMapName();
	//레벨이름에 _ 접두사를 위한 처리
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (CurrentLevel == "MH_LV_MainUI")
	{
		if (MainUIClass)
		{
			MainUI = CreateWidget<UMH_W_MainUI>(this, MainUIClass);

			if (MainUI)
			{
				MainUI->AddToViewport();
				SetInputMode(FInputModeUIOnly());
				bShowMouseCursor = true;
			}
		}
	
	}
	else
	{
		if (InGameRootClass)
		{
			InGameUIRoot = CreateWidget<UMH_W_InGameUIRoot>(this, InGameRootClass);
			if (InGameUIRoot)
			{
				InGameUIRoot->AddToViewport();

				OnIngameMenuPressed.AddDynamic(InGameUIRoot,&UMH_W_InGameUIRoot::ShowInGameMenu);

				if (ASBGameMode* GM = Cast<ASBGameMode>(UGameplayStatics::GetGameMode(this)))
				{
					GM->OnGameOver.AddDynamic(this, &AMH_SBPlayerController::GameOver);
					GM->OnGameClear.AddDynamic(this, &AMH_SBPlayerController::GameClear);
				}
			}
		}
		else
		{
		}
		if (!GetPawn())
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
			if (PlayerPawn)
			{
				Possess(PlayerPawn);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Pawn 찾기 실패"));
			}
		}
		
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
	
}
