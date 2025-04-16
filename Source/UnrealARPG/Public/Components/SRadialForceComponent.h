// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SRadialForceComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent)) 
class UNREALARPG_API USRadialForceComponent : public URadialForceComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Impulse)
	AActor* Instigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Impulse)
	uint32 bIgnoreInstigator:1;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
