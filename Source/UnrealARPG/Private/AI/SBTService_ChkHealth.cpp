// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_ChkHealth.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/AIModule/Classes/AIController.h>
#include "Components/SAttributeComponent.h"

void USBTService_ChkHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* OwnerBlackBordComp = OwnerComp.GetBlackboardComponent();

	if (OwnerBlackBordComp) {

		AAIController* OwnerControl = OwnerComp.GetAIOwner();

		if (OwnerControl) {

			APawn* OwnerPawn = OwnerControl->GetPawn();

			if (OwnerPawn) {
				
				USAttributeComponent* AttrComp = Cast<USAttributeComponent>(OwnerPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

				if (AttrComp){
					
					OwnerBlackBordComp->SetValueAsBool(m_HealthLowKey.SelectedKeyName, AttrComp->GetCurHealth() <= m_LowHealth);
				}
			}
		}
	}
}
