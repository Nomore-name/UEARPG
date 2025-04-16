// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SEnemyDataAsset.generated.h"

class USAction;

/**
 * 
 */
UCLASS()
class UNREALARPG_API USEnemyDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TArray<TSubclassOf<USAction>> Actions;

	FPrimaryAssetId GetPrimaryAssetId()	const override
	{
		return FPrimaryAssetId("Enemy", GetFName());
	}
	
};
