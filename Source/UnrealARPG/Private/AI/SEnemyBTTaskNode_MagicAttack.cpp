// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SEnemyBTTaskNode_MagicAttack.h"
#include <Runtime/AIModule/Classes/AIController.h>
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "Action/SActionComponent.h"


EBTNodeResult::Type USEnemyBTTaskNode_MagicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();

	if (OwnerController) {

		ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerController->GetPawn());

		if (OwnerCharacter) {
			
			FTimerHandle TimeHandleAttack;
			FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &USEnemyBTTaskNode_MagicAttack::SpawnMagicAttack, OwnerController);

			OwnerCharacter->PlayAnimMontage(m_AttackAnim);
			OwnerCharacter->GetWorldTimerManager().SetTimer(TimeHandleAttack, RespawnDelegate, 0.2f, false);
			
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

void USEnemyBTTaskNode_MagicAttack::SpawnMagicAttack(AAIController* OwnerController)
{
	if (!OwnerController) {
		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerController->GetPawn());

	if (OwnerCharacter) {

		FVector HandLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		AActor* TargetActor = Cast<AActor>(OwnerController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));

		if (TargetActor) {

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Instigator = OwnerCharacter;

			float RandSpread = m_MaxRandSpread;
			USActionComponent* ActionComp = Cast<USActionComponent>(OwnerCharacter->GetComponentByClass(USActionComponent::StaticClass()));
			if (ActionComp) {
				if (ActionComp->IsAciveTag(FGameplayTag::RequestGameplayTag(FName("DeBuff.Black")))) {
					RandSpread += 10.0f;
				}
			}

			FRotator SpawnRotator = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetActor->GetActorLocation());
			SpawnRotator.Pitch += FMath::RandRange(0.0f, RandSpread);
			SpawnRotator.Yaw += FMath::RandRange(-RandSpread, RandSpread);

			FTransform SpawnTrans = FTransform(SpawnRotator, HandLocation);
			
			GetWorld()->SpawnActor<AActor>(m_ProjectileClass, SpawnTrans, SpawnParams);
		}
	}
}
