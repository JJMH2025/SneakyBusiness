// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LHM/Enemy/Enemy.h"
#include "Enemy_ShooterFlare.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYBUSINESS_API AEnemy_ShooterFlare : public AEnemy
{
	GENERATED_BODY()

public:
	AEnemy_ShooterFlare();

	virtual void Attack() override;
	virtual void Signal() override;
	
};
