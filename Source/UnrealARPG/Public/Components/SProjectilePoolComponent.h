// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SProjectilePoolComponent.generated.h"

class ASProjectileBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALARPG_API USProjectilePoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USProjectilePoolComponent();

	ASProjectileBase* GetFirstAliveProjectile();

	ASProjectileBase* SpawnFromPool(FTransform SpawnTransform);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TArray<ASProjectileBase*> m_ProjectilePool;

public:	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> m_ProjectileClass;
};
