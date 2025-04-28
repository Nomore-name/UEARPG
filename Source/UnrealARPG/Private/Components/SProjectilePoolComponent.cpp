// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SProjectilePoolComponent.h"
#include "Projectile/SProjectileBase.h"
#include "../UnrealARPG.h"

// Sets default values for this component's properties
USProjectilePoolComponent::USProjectilePoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


ASProjectileBase* USProjectilePoolComponent::GetFirstAliveProjectile()
{
	for (auto Projectile : m_ProjectilePool) {
		if (!Projectile->IsUsed()) {
			return Projectile;
		}
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Cast<APawn>(GetOwner());

	ASProjectileBase* NewProjectile = Cast<ASProjectileBase>(GetWorld()->SpawnActor<AActor>(m_ProjectileClass, SpawnParams));
	if (NewProjectile) {
		m_ProjectilePool.Add(NewProjectile);
	}

	return NewProjectile;
}

ASProjectileBase* USProjectilePoolComponent::SpawnFromPool(FTransform SpawnTransform)
{
	ASProjectileBase* Projectile = GetFirstAliveProjectile();
	if (Projectile) {
		Projectile->SetActorTransform(SpawnTransform);
		Projectile->SetIsUse(true);
	}

	return Projectile;
}

// Called when the game starts
void USProjectilePoolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
