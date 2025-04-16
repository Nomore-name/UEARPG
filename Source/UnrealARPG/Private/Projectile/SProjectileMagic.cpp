// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SProjectileMagic.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include <Action/SActionComponent.h>
#include <Action/SDebuffAction.h>
#include <../UnrealARPG.h>
#include "Game/SGamePlayFunctionLibrary.h"

ASProjectileMagic::ASProjectileMagic()
{
	m_PjMoveCom->ProjectileGravityScale = 0.0f;
	m_PjMoveCom->bRotationFollowsVelocity = true;		//使发射物旋转跟随速度方向
	m_PjMoveCom->bInitialVelocityInLocalSpace = true;	//使初始速度是以本地坐标系发射
	m_PjMoveCom->InitialSpeed = 1000.0f;
}

void ASProjectileMagic::BeginPlay()
{
	Super::BeginPlay();
	m_SphereCom->IgnoreActorWhenMoving(GetInstigator(), true);				//忽略事件引发者
}

void ASProjectileMagic::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	
	if (!OtherActor) {
		return;
	}

	if (GetInstigator() == OtherActor) {
		return;
	}

	/*USAttributeComponent* AttrComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttrComp) {
		AttrComp->AddHealth(GetInstigator(), m_Damage);
	}

	Destroy();*/

	USGamePlayFunctionLibrary::ApplyAnger(GetInstigator(), OtherActor, m_AddAnger);
	bool bDamage = USGamePlayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, m_Damage, Hit);

	//BUFF添加只运行在服务端的逻辑
	if (bDamage && m_DamageEffect != nullptr && HasAuthority()) {
		USActionComponent* InstigatorAction = Cast<USActionComponent>(GetInstigator()->GetComponentByClass(USActionComponent::StaticClass()));
		if (InstigatorAction && InstigatorAction->IsAciveTag(FGameplayTag::RequestGameplayTag(FName("Buff.Fire")))) {
			USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
			if (ActionComp) {
				ActionComp->AddAction(GetInstigator(), m_DamageEffect);
			}
		}
	}

	Destroy();
}

