// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_ChkHealth.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARPG_API USBTService_ChkHealth : public UBTService
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector m_HealthLowKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float m_LowHealth = 40.0f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
