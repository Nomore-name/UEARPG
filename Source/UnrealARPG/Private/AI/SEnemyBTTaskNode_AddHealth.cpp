// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SEnemyBTTaskNode_AddHealth.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include <Runtime/AIModule/Classes/AIController.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Components/SAttributeComponent.h"


EBTNodeResult::Type USEnemyBTTaskNode_AddHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();

	if (OwnerController) {

		ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerController->GetPawn());

		if (OwnerCharacter) {

			USAttributeComponent* AttrComp = Cast<USAttributeComponent>(OwnerCharacter->GetComponentByClass(USAttributeComponent::StaticClass()));

			if (AttrComp) {
				OwnerCharacter->PlayAnimMontage(m_AttackAnim);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_EmittParticle, OwnerCharacter->GetActorTransform() + m_ParTransform);
				AttrComp->AddHealth(OwnerCharacter, m_AddHealth);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
