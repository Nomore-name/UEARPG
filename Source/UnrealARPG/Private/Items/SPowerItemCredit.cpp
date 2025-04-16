// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SPowerItemCredit.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Player/SPlayerState.h"

ASPowerItemCredit::ASPowerItemCredit()
{
	m_MeshBaseComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	m_MeshBaseComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshBaseComp->SetupAttachment(RootComponent);
}

void ASPowerItemCredit::Interact_Implementation(APawn* Target)
{
	ASPlayerState *PlayerState = Cast<ASPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	if (!PlayerState) {
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is nullptr"));
		return;
	}

	PlayerState->IncreaseScore(m_AddCredits);
	Destroy();
}
