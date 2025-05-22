// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Templates/SharedPointer.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineStats.h"
#include "SBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API USBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BluePrintReadWrite)
	int32 ToatlScore = 0;

	UPROPERTY(BluePrintReadWrite)
	int32 CurrentStageIndex = 1;

	void SetTotalScore(int32 ScoreToAdd) { ToatlScore += ScoreToAdd; }

// Rank System - Steam Leaderboard 연동 관련
public:
	TSharedRef<class FOnlineLeaderboardRead> LeaderboardRead = MakeShared<class FOnlineLeaderboardRead>();

	//TSharedPtr<class FOnlineLeaderboardRead> LeaderboardRead;
	FDelegateHandle ReadCompleteHandle;

	// 점수 업로드 함수
	void UploadScoreToLeaderboard(int32 Score);

	// 상위 랭커 조회
	void ReadTopRankers();

	// 랭커 조회 완료시 호출될 콜백
	void OnLeaderboardReadComplete(bool bWasSuccessful);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> RankResultWidgetClass;
};
