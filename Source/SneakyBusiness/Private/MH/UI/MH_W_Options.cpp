// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_Options.h"

#include "Components/Button.h"
#include "Components/Slider.h"

void UMH_W_Options::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Back)
	{
		Btn_Back->OnClicked.AddDynamic(this,&UMH_W_Options::OnBackClicked);
	}

	if (Btn_Save)
	{
		Btn_Save->OnClicked.AddDynamic(this,&UMH_W_Options::OnSaveClicked);
	}

	if (Btn_Credits)
	{
		Btn_Credits->OnClicked.AddDynamic(this,&UMH_W_Options::OnCreditsClicked);
	}

	if (Sli_BGM)
	{
		Sli_BGM->OnValueChanged.AddDynamic(this,&UMH_W_Options::OnBGMVolumeChanged);
	}
	
	if (Sli_SFX)
	{
		Sli_SFX->OnValueChanged.AddDynamic(this,&UMH_W_Options::OnSFXVolumeChanged);
	}

	//GI에서 볼륨 초기값 불러오기
	//SoundClass 만들기?
	
}

void UMH_W_Options::OnSaveClicked()
{
	//GI에 변경 볼륨값 적용 
}

void UMH_W_Options::OnBackClicked()
{
}

void UMH_W_Options::OnCreditsClicked()
{
}

void UMH_W_Options::OnBGMVolumeChanged(float Value)
{
}

void UMH_W_Options::OnSFXVolumeChanged(float Value)
{
}
