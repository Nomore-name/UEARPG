// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SAction.h"
#include "SDebuffAction.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARPG_API USDebuffAction : public USAction
{
	GENERATED_BODY()

public:
	USDebuffAction();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() const;

protected:
	FTimerHandle m_EffectTimeHandle;
	FTimerHandle m_PeriodTimeHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float m_Duration = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float m_Period = 5.f;

	UFUNCTION(BluePrintNativeEvent, Category = "Effect")
	void ExecutePeriodEffect(AActor* Instigator);
	
};
