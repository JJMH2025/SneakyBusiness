// Fill out your copyright notice in the Description page of Project Settings.


#include "LHM/UI/GameUI.h"
#include "LHM/GameSystem/SBGameState.h"
#include "LHM/GameSystem/SBGameInstance.h"
#include "MH/Player_Nick.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"

void UGameUI::NativeConstruct()
{
    Super::NativeConstruct();
    InitUI();
}

void UGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    UpdateUI();
}

void UGameUI::InitUI()
{
    HPSlots.Empty();
    ItemSlots.Empty();

    if (APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        for (int32 i = 0; i < Player->MaxHP; ++i)
        {
            UImage* HPImg = NewObject<UImage>(this);
            HPImg->SetBrushFromTexture(HPImage);
            HPImg->SetOpacity(1.0f);
            
            USizeBox* SizeBox = NewObject<USizeBox>(this);
            SizeBox->SetWidthOverride(100.0f);
            SizeBox->SetHeightOverride(100.0f);
            SizeBox->AddChild(HPImg);

            UHorizontalBoxSlot* HPSlot = HorzBox_HPContainer->AddChildToHorizontalBox(SizeBox);
            HPSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
            HPSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
            HPSlot->SetPadding(FMargin(2.0f));

            HPSlots.Add(HPImg);
        }
    }

    if (ASBGameState* GS = GetWorld()->GetGameState<ASBGameState>())
    {
		for (int32 i = 0; i < GS->RequiredItemCount; ++i)
		{
			UImage* ItemImg = NewObject<UImage>(this);
			ItemImg->SetBrushFromTexture(StolenItemImage);
			
            USizeBox* SizeBox = NewObject<USizeBox>(this);
            SizeBox->SetWidthOverride(100.0f);
            SizeBox->SetHeightOverride(100.0f);
            SizeBox->AddChild(ItemImg);

            UHorizontalBoxSlot* ItemSlot = HorzBox_StolenItemContainer->AddChildToHorizontalBox(SizeBox);
            ItemSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
            ItemSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
            ItemSlot->SetPadding(FMargin(2.0f));

			ItemSlots.Add(ItemImg);
		}
    }
}

void UGameUI::UpdateUI()
{
    ASBGameState* GS = GetWorld()->GetGameState<ASBGameState>();
    USBGameInstance* GI = GetWorld()->GetGameInstance<USBGameInstance>();
    APlayer_Nick* Player = Cast<APlayer_Nick>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (!GS || !GI || !Player) return;
    
    // 타이머
    float RemainingTime = FMath::Max(0.f, GS->TimeLimit - GS->ElapsedTime);

    Txt_Timer->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), RemainingTime)));

    // 스테이지 넘버
    Txt_StageNum->SetText(FText::FromString(FString::Printf(TEXT("STAGE %d"), GI->CurrentStageIndex)));

    // 스코어
    Txt_Score->SetText(FText::AsNumber(GS->CurrentScore));

    // 플레이어 HP
    for (int32 i = 0; i < HPSlots.Num(); ++i)
    {
        if (i < Player->CurrentPlayerHP)
            HPSlots[i]->SetOpacity(1.0f);
        else
            HPSlots[i]->SetOpacity(0.2f);
    }

    // 훔칠 물건 리스트
    for (int32 i = 0; i < ItemSlots.Num(); ++i)
    {
        if (i < GS->CollectedItemCount)
            ItemSlots[i]->SetOpacity(1.0f);
        else
            ItemSlots[i]->SetOpacity(0.2f);
    }
}
