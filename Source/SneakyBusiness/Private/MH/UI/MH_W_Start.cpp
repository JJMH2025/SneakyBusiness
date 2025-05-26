// Fill out your copyright notice in the Description page of Project Settings.


#include "MH/UI/MH_W_Start.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMH_W_Start::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Tutorial)
	{
		Btn_Tutorial->OnClicked.AddDynamic(this,&UMH_W_Start::OnTutorialClicked);
	}
	
	if (Btn_Stage01)
	{
		Btn_Stage01->OnClicked.AddDynamic(this,&UMH_W_Start::OnStage01Clicked);
	}

	if (Btn_Stage02)
	{
		Btn_Stage02->OnClicked.AddDynamic(this,&UMH_W_Start::OnStage02Clicked);
	}

	if (Btn_Stage03)
	{
		Btn_Stage03->OnClicked.AddDynamic(this,&UMH_W_Start::OnStage03Clicked);
	}
	
	if (Btn_Stage04)
	{
		Btn_Stage04->OnClicked.AddDynamic(this,&UMH_W_Start::OnStage04Clicked);
	}

	if (Btn_Back)
	{
		Btn_Back->OnClicked.AddDynamic(this,&UMH_W_Start::OnBackClicked);
	}
}

void UMH_W_Start::OnTutorialClicked()
{
	
}

void UMH_W_Start::OnStage01Clicked()
{
	UGameplayStatics::OpenLevel(this,FName("MH_LV_Stage01"));
}

void UMH_W_Start::OnStage02Clicked()
{
	//if 스테이지 1을 클리어 했다면. 클리어 스테이지 정보 GI에서 받아오기
	UGameplayStatics::OpenLevel(this,FName("MH_LV_Stage02"));
}

void UMH_W_Start::OnStage03Clicked()
{
	
}

void UMH_W_Start::OnStage04Clicked()
{
	
}

void UMH_W_Start::OnBackClicked()
{
	OnBackPressed.Broadcast();
}
