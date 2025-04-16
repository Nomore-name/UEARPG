// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SEnemyBTService_ChkRange.generated.h"

/**
 * 
 */
struct FBlackboardKeySelector;
UCLASS()
class UNREALARPG_API USEnemyBTService_ChkRange : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector m_AttackRangeKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
