// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/SDefenseAction.h"
#include <../UnrealARPG.h>
#include <Action/SActionComponent.h>
#include "Character/SCharacter.h"
#include "Character/SShield.h"

USDefenseAction::USDefenseAction()
{
	m_ActionName = FName("Defense");

	FGameplayTag TagRun = FGameplayTag::RequestGameplayTag(FName("Action.Running"));
	FGameplayTag TagAtk = FGameplayTag::RequestGameplayTag(FName("Action.Attacking"));
	FGameplayTag TagDef = FGameplayTag::RequestGameplayTag(FName("Action.Defending"));

	m_GrantTags.AddTag(TagDef);
	m_BlockTags.AddTag(TagAtk);
	m_BlockTags.AddTag(TagRun);
}

void USDefenseAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	OnShieldDefense.Broadcast(Instigator, true);

	ASCharacter* Character = Cast<ASCharacter>(Instigator);

	if (!Character) {
		return;
	}

	UChildActorComponent* ChildDefenseComp = Character->GetChildDefenseComp();

	if (!ChildDefenseComp) {
		return;
	}

	ChildDefenseComp->Activate();
	ChildDefenseComp->SetVisibility(true);

	ASShield* Shield = Cast<ASShield>(ChildDefenseComp->GetChildActor());

	if (!Shield) {
		return;
	}

	Shield->SetShieldCollision(true);
	Shield->SetShieldRegenate(false);
}

void USDefenseAction::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	OnShieldDefense.Broadcast(Instigator, false);

	ASCharacter* Character = Cast<ASCharacter>(Instigator);

	if (!Character) {
		return;
	}

	UChildActorComponent* ChildDefenseComp = Character->GetChildDefenseComp();

	if (!ChildDefenseComp) {
		return;
	}

	ChildDefenseComp->Deactivate();
	ChildDefenseComp->SetVisibility(false);

	ASShield* Shield = Cast<ASShield>(ChildDefenseComp->GetChildActor());

	if (!Shield) {
		return;
	}

	Shield->SetShieldCollision(false);
	Shield->SetShieldRegenate(true);
}
