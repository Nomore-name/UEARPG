// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/SRunAction.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include <Runtime/GameplayTags/Classes/GameplayTagContainer.h>

USRunAction::USRunAction()
{
	m_ActionName = FName("Run");

	FGameplayTag TagRun = FGameplayTag::RequestGameplayTag(FName("Action.Running"));
	FGameplayTag TagAtk = FGameplayTag::RequestGameplayTag(FName("Action.Attacking"));
	FGameplayTag TagDef = FGameplayTag::RequestGameplayTag(FName("Action.Defending"));

	m_GrantTags.AddTag(TagRun);
	m_BlockTags.AddTag(TagAtk);
	m_BlockTags.AddTag(TagDef);
}

void USRunAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (!Instigator) {
		return;
	}

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (!Character){
		return;
	}

	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();

	if (!MoveComp) {
		return;
	}

	MoveComp->MaxWalkSpeed = MoveComp->MaxWalkSpeed + m_AddSpeed;
}

void USRunAction::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	
	if (!Instigator) {
		return;
	}

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (!Character) {
		return;
	}

	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();

	if (!MoveComp) {
		return;
	}

	MoveComp->MaxWalkSpeed = MoveComp->MaxWalkSpeed - m_AddSpeed;
}
