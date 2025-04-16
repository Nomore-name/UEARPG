// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SAction.h"
#include "SRunAction.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARPG_API USRunAction : public USAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	float m_AddSpeed = 1000.0f;

public:
	USRunAction();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

};
