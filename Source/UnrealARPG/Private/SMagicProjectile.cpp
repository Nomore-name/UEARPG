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
	//m_SphereCom->SetCollisionObjectType(ECC_WorldDynamic);				//������ײ����
	//m_SphereCom->SetCollisionResponseToAllChannels(ECR_Ignore);			//���ö�����������ײ����Ӧ
	//m_SphereCom->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);	//���ö�ĳ��������ײ����Ӧ
	m_SphereCom->SetCollisionProfileName("Projectile");						//ͨ���Զ����ļ���Ϣ������ײ��Ӧ

	m_PjMoveCom->ProjectileGravityScale = 0.0f;
	m_PjMoveCom->bRotationFollowsVelocity = true;		//ʹ��������ת�����ٶȷ���
	m_PjMoveCom->bInitialVelocityInLocalSpace = true;	//ʹ��ʼ�ٶ����Ա�������ϵ����
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
