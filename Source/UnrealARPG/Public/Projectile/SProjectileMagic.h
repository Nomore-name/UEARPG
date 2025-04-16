// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SProjectileMagic.generated.h"

class USDebuffAction;

/**
 * 
 */
UCLASS()
class UNREALARPG_API ASProjectileMagic : public ASProjectileBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASProjectileMagic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float m_Damage = -20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float m_AddAnger = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<USDebuffAction> m_DamageEffect;

private:
	FTimerHandle m_DestorySelfTimeHandle;
	
};
