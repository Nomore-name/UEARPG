// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerItemBase.h"
#include "SPowerItemHealth.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class UNREALARPG_API ASPowerItemHealth : public ASPowerItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPowerItemHealth();

protected:
	UPROPERTY(EditAnywhere)
	float m_AddHealth = 20.f;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_MeshBaseComp = nullptr;

public:
	void Interact_Implementation(APawn* Target) override;
	
};
