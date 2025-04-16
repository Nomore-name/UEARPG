// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARPG_API ASEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* m_EnemyBehaviorTree;

	virtual void BeginPlay() override;
};
