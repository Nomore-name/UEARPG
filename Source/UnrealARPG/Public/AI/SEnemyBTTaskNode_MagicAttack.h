// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SEnemyBTTaskNode_MagicAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALARPG_API USEnemyBTTaskNode_MagicAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> m_ProjectileClass;

	UPROPERTY(EditAnywhere, Category="AI")
	UAnimMontage* m_AttackAnim = nullptr;

	UPROPERTY(EditAnywhere, Category="AI")
	float m_MaxRandSpread = 8.0f;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FActorSpawnParameters m_SpawnParams;
	FTransform m_SpawnTrans;

	void SpawnMagicAttack(AAIController* OwnerController);
};
