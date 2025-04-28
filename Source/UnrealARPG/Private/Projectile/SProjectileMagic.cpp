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
}

void ASProjectileMagic::SetIsUse(bool bUse)
{
	Super::SetIsUse(bUse);

	if (bUse) {
		m_PjMoveCom->Velocity = 2000.f * GetActorForwardVector();
	}else {
		m_PjMoveCom->StopMovementImmediately();
	}

}

void ASProjectileMagic::BeginPlay()
{
	Super::BeginPlay();
	SetIsUse(false);
	m_SphereCom->IgnoreActorWhenMoving(GetInstigator(), true);				//忽略事件引发者
}

void ASProjectileMagic::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) {
		return;
	}

	if (GetInstigator() == OtherActor) {
		return;
	}

	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	USGamePlayFunctionLibrary::ApplyAnger(GetInstigator(), OtherActor, m_AddAnger);
	bool bDamage = USGamePlayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, m_Damage, SweepResult);

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

	SetIsUse(false);
}
