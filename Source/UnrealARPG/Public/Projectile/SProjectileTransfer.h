// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SProjectileTransfer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARPG_API ASProjectileTransfer : public ASProjectileBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TransferDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay = 0.2f;

	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void Explode_Implementation() override;

	virtual void BeginPlay() override;

	void TransferInstigator();

public:
	ASProjectileTransfer();
};
