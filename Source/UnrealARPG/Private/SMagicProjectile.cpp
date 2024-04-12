// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_SphereCom = CreateDefaultSubobject<USphereComponent>("SphereCom");
	m_ParticleSysCom = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemCom");
	m_PjMoveCom = CreateDefaultSubobject<UProjectileMovementComponent>("ProjecMoveCom");

	RootComponent = m_SphereCom;
	m_ParticleSysCom->SetupAttachment(m_SphereCom);
	//m_SphereCom->SetCollisionObjectType(ECC_WorldDynamic);				//设置碰撞类型
	//m_SphereCom->SetCollisionResponseToAllChannels(ECR_Ignore);			//设置对所有类型碰撞的响应
	//m_SphereCom->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);	//设置对某个类型碰撞的响应
	m_SphereCom->SetCollisionProfileName("Projectile");						//通过自定义文件信息设置碰撞相应

	m_PjMoveCom->ProjectileGravityScale = 0.0f;
	m_PjMoveCom->bRotationFollowsVelocity = true;		//使发射物旋转跟随速度方向
	m_PjMoveCom->bInitialVelocityInLocalSpace = true;	//使初始速度是以本地坐标系发射
	m_PjMoveCom->InitialSpeed = 1000.0f;
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

