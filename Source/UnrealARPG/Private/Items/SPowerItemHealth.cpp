// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SPowerItemHealth.h"
#include "Components/SAttributeComponent.h"

ASPowerItemHealth::ASPowerItemHealth()
{
	m_MeshBaseComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	m_MeshBaseComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshBaseComp->SetupAttachment(RootComponent);
}

void ASPowerItemHealth::Interact_Implementation(APawn* Target)
{
	if (!Target) {
		return;
	}

	USAttributeComponent* AttrComp = USAttributeComponent::GetAttributeComponent(Target);

	if (AttrComp) {
		if (AttrComp->AddHealth(Target, m_AddHealth)) {
			HideAndCooldownItem();
		}
	}
}

