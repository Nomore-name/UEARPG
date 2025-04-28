// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SProjectileBase.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Components/AudioComponent.h>
#include <Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <Runtime/Engine/Classes/Sound/SoundCue.h>
#include "Runtime/Engine/Public/Net/UnrealNetwork.h"
#include "../UnrealARPG.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
	LogOnScreen(GetWorld(), FString::Printf(TEXT("ASProjectileBase %d"), GetLocalRole()), FColor::Red);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_SphereCom = CreateDefaultSubobject<USphereComponent>("SphereCom");
	m_SphereCom->SetCollisionProfileName("Projectile");	
	m_SphereCom->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnComponentBeginOverlap);
	RootComponent = m_SphereCom;

	m_ParticleSysCom = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemCom");
	m_ParticleSysCom->SetupAttachment(m_SphereCom);

	m_AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	m_AudioComp->SetupAttachment(RootComponent);

	m_PjMoveCom = CreateDefaultSubobject<UProjectileMovementComponent>("ProjecMoveCom");
	m_PjMoveCom->bRotationFollowsVelocity = true;
	m_PjMoveCom->bInitialVelocityInLocalSpace = true;
	m_PjMoveCom->ProjectileGravityScale = 0.0f;
	m_PjMoveCom->InitialSpeed = 0.0f;
	
	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASProjectileBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode_Implementation();
}

void ASProjectileBase::Explode_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_EmittParticle, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(this, m_ImpactSound, GetActorLocation());
	UGameplayStatics::PlayWorldCameraShake(this,m_ImpactShake, GetActorLocation(), m_ImpactShakeInnerRadius, m_ImpactShakeOuterRadius);
}

void ASProjectileBase::OnRep_Used()
{
	SetActorEnableCollision(m_bUsed);
	SetActorHiddenInGame(!m_bUsed);
	SetActorTickEnabled(m_bUsed);

	if (m_ParticleSysCom) {
		m_ParticleSysCom->SetActive(m_bUsed);
	}

	if (m_bUsed) {
		GetWorldTimerManager().ClearTimer(m_HideTimeHandle);
		GetWorldTimerManager().SetTimer(m_HideTimeHandle, this, &ASProjectileBase::HideSelf, m_AliveTime);
	}
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASProjectileBase::HideSelf()
{
	SetIsUse(false);
}

// Called every frame
void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASProjectileBase::SetIsUse(bool bUse)
{
	if (HasAuthority()) {
		m_bUsed = bUse;
		OnRep_Used();
	}
}

bool ASProjectileBase::IsUsed() const
{
	return m_bUsed;
}

void ASProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASProjectileBase, m_bUsed);
}

