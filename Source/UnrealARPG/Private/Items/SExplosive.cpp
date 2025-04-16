// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SExplosive.h"
#include "Runtime/Engine/Classes/PhysicsEngine/RadialForceComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Projectile/SProjectileMagic.h"

// Sets default values
ASExplosive::ASExplosive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_RadialForceCom = CreateDefaultSubobject<URadialForceComponent>("RadialForceCompoment");
	m_ExplosiveMeshCom = CreateDefaultSubobject<UStaticMeshComponent>("ExplosiveMesh");

	RootComponent = m_ExplosiveMeshCom;
	m_RadialForceCom->SetupAttachment(m_ExplosiveMeshCom);
}

// Called when the game starts or when spawned
void ASExplosive::BeginPlay()
{
	Super::BeginPlay();

	//需放在BeginPlay中绑定委托，否则生成蓝图类会导致绑定失效，无法触发	
	m_ExplosiveMeshCom->OnComponentHit.AddDynamic(this, &ASExplosive::OnComponentHitEvent);
}

void ASExplosive::OnComponentHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ASProjectileMagic* MagicProjectile = Cast<ASProjectileMagic>(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("Collision detected: %s"), *OtherActor->GetName());

	if (m_ExplosiveMeshCom) {
		m_ExplosiveMeshCom->SetScalarParameterValueOnMaterials("HitTime", GetWorld()->GetTimeSeconds());
	}

	if (MagicProjectile) {
		m_RadialForceCom->FireImpulse();
		MagicProjectile->Destroy();
	}

}

// Called every frame
void ASExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

