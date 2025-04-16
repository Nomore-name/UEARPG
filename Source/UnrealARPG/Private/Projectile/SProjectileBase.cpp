// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SProjectileBase.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Components/AudioComponent.h>
#include <Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <Runtime/Engine/Classes/Sound/SoundCue.h>

// Sets default values
ASProjectileBase::ASProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_SphereCom = CreateDefaultSubobject<USphereComponent>("SphereCom");
	m_SphereCom->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnComponentHit);
	m_SphereCom->SetCollisionProfileName("Projectile");								//通过自定义文件信息设置碰撞相应
	RootComponent = m_SphereCom;

	m_ParticleSysCom = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemCom");
	m_ParticleSysCom->SetupAttachment(m_SphereCom);

	m_AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	m_AudioComp->SetupAttachment(RootComponent);

	m_PjMoveCom = CreateDefaultSubobject<UProjectileMovementComponent>("ProjecMoveCom");
	m_PjMoveCom->bRotationFollowsVelocity = true;
	m_PjMoveCom->bInitialVelocityInLocalSpace = true;
	m_PjMoveCom->ProjectileGravityScale = 0.0f;
	m_PjMoveCom->InitialSpeed = 8000;
	
	m_ImpactShakeInnerRadius = 0.0f;
	m_ImpactShakeOuterRadius = 1500.0f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(m_DestorySelfTimeHandle, this, &ASProjectileBase::DestorySelf, 10.0f);
}

void ASProjectileBase::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode_Implementation();
}

void ASProjectileBase::Explode_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_EmittParticle, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(this, m_ImpactSound, GetActorLocation());
	UGameplayStatics::PlayWorldCameraShake(this,m_ImpactShake, GetActorLocation(), m_ImpactShakeInnerRadius, m_ImpactShakeOuterRadius);
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASProjectileBase::DestorySelf()
{
	GetWorldTimerManager().ClearTimer(m_DestorySelfTimeHandle);
	Destroy();
}

// Called every frame
void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

