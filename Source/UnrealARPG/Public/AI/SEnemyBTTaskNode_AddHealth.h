// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SEnemyBTTaskNode_AddHealth.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class UNREALARPG_API USEnemyBTTaskNode_AddHealth : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category="AI")
	float m_AddHealth = 40.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	FTransform m_ParTransform;

	UPROPERTY(EditAnywhere, Category = "AI")
	UParticleSystem* m_EmittParticle = nullptr;

	UPROPERTY(EditAnywhere, Category="AI")
	UAnimMontage* m_AttackAnim = nullptr;
};
