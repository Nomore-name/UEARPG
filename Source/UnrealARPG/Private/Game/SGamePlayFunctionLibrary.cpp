// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGamePlayFunctionLibrary.h"
#include "Components/SAttributeComponent.h"

bool USGamePlayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(TargetActor->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (AttributeComp) {
		return AttributeComp->AddHealth(DamageCauser, DamageAmount);
	}

	return false;
}

bool USGamePlayFunctionLibrary::ApplyAnger(AActor* DamageCauser, AActor* TargetActor, float AngerVal)
{
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(TargetActor->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (AttributeComp) {
		return AttributeComp->AddAnger(DamageCauser, AngerVal);
	}

	return false;
}

bool USGamePlayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount)) {

		//UPrimitiveComponent是静态网格体和骨骼网格体上每一个骨骼对应的网格体组件所继承的父类。ࡣ
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		if (HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName)) {
			HitComponent->AddImpulseAtLocation(HitResult.ImpactNormal * -30000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}

		return true;
	}

	return false;
}
