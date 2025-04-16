// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerItemBase.h"
#include "SPowerItemCredit.generated.h"

class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class UNREALARPG_API ASPowerItemCredit : public ASPowerItemBase
{
	GENERATED_BODY()

public:
	ASPowerItemCredit();

protected:
	UPROPERTY(EditAnywhere)
	int32 m_AddCredits = 5;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_MeshBaseComp = nullptr;

public:
	void Interact_Implementation(APawn* Target) override;
	
};
