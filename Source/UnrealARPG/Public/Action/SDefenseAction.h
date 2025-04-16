// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SAction.h"
#include "SDefenseAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldDefense, AActor*, InstigatorActor, bool, bDefense);
/**
 * 
 */
UCLASS()
class UNREALARPG_API USDefenseAction : public USAction
{
	GENERATED_BODY()
	
public:
	USDefenseAction();
	
	UPROPERTY(BlueprintAssignable)
	FOnShieldDefense OnShieldDefense;
protected:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
};
