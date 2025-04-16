// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SShield.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include <Components/SAttributeComponent.h>

// Sets default values
ASShield::ASShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_BaseMeshCom = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshCom");
	RootComponent = m_BaseMeshCom;

	m_AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

// Called when the game starts or when spawned
void ASShield::BeginPlay()
{
	Super::BeginPlay();
}

void ASShield::RegenateShield()
{
	if (m_AttributeComp && m_AttributeComp->GetMaxHealth() != m_AttributeComp->GetCurHealth()) {
		m_AttributeComp->AddHealth(this, m_RegValue);
	}
}

void ASShield::SetShieldCollision(bool bOpen)
{
	if (m_BaseMeshCom) {
		m_BaseMeshCom->SetCollisionEnabled(bOpen ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void ASShield::SetShieldRegenate(bool bStart)
{
	if (bStart) {
		GetWorld()->GetTimerManager().SetTimer(m_TimeHandleRegShield, this, &ASShield::RegenateShield, m_RegRate, true);
	} else {
		GetWorld()->GetTimerManager().ClearTimer(m_TimeHandleRegShield);
	}
}

USAttributeComponent* ASShield::GetAttributeComp() const
{
	return m_AttributeComp;
}

// Called every frame
void ASShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

