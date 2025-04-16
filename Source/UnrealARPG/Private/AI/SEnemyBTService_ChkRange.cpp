// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SEnemyBTService_ChkRange.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/AIModule/Classes/AIController.h>
#include "Components/SAttributeComponent.h"
#include "Character/SShield.h"

void USEnemyBTService_ChkRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* OwnerBlackBordComp = OwnerComp.GetBlackboardComponent();

	if (OwnerBlackBordComp) {

		AActor* TargetActor = Cast<AActor>(OwnerBlackBordComp->GetValueAsObject("TargetActor"));

		if (TargetActor) {
			
			AAIController* OwnerControl = OwnerComp.GetAIOwner();

			if (OwnerControl) {
				
				APawn* OwnerPawn = OwnerControl->GetPawn();

				if (OwnerPawn) {

					USAttributeComponent* AttrComp = Cast<USAttributeComponent>(TargetActor->GetComponentByClass(USAttributeComponent::StaticClass()));
					if (!AttrComp || AttrComp->GetCurHealth() <= 0) {
						//检测目标是否存活
						OwnerBlackBordComp->SetValueAsBool(m_AttackRangeKey.SelectedKeyName, false);
						return;
					}

					float TargetDis = FVector::Distance(OwnerPawn->GetActorLocation(), TargetActor->GetActorLocation());
					bool bInRange = TargetDis < 1000.0f;
					bool bInSight = false;
					
					TArray<AActor*> ChildActors;
					TargetActor->GetAttachedActors(ChildActors); 
					ASShield* Shield = nullptr;

					for (AActor* Child : ChildActors) {
						Shield = Cast<ASShield>(Child);
						if (Shield) {
							break;
						}
					}

					if (bInRange) {
						bInSight = OwnerControl->LineOfSightTo(TargetActor);
						if (Shield) {
							//检测是否打在盾牌上
							bInSight = bInSight || OwnerControl->LineOfSightTo(Shield);
						}
					}

					OwnerBlackBordComp->SetValueAsBool(m_AttackRangeKey.SelectedKeyName, bInSight);
				}
			}

		}
	}
}
