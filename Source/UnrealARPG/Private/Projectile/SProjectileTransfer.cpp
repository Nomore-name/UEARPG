// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SProjectileTransfer.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ASProjectileTransfer::ASProjectileTransfer()
{
	m_PjMoveCom->InitialSpeed = 6000.f;
}

void ASProjectileTransfer::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASProjectileTransfer::Explode, DetonateDelay);
}

void ASProjectileTransfer::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_EmittParticle, GetActorTransform());

	m_ParticleSysCom->DeactivateSystem();			//停用系统
	m_PjMoveCom->StopMovementImmediately();

	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTransfer;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTransfer, this, &ASProjectileTransfer::TransferInstigator, TransferDelay);

}

void ASProjectileTransfer::TransferInstigator()
{
	AActor* TransActor = GetInstigator();
	if (TransActor) {
		TransActor->TeleportTo(GetActorLocation(), TransActor->GetActorRotation());
	}
}
