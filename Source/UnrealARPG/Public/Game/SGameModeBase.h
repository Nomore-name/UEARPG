// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <Runtime/AIModule/Classes/EnvironmentQuery/EnvQueryTypes.h>
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FEnemyInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId EnemyID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpwanWight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward = 5.0f;
};

/**
 * 
 */
UCLASS()
class UNREALARPG_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override; 

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	enum {	POWER_ITEM_NUM = 3, POWER_ITEM_SPAWN_NUM = 10 };
	
	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable)
	void OnActorKilled(AActor *DiedActor, AActor *KillActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UEnvQuery* m_EQSpowerItem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	UEnvQuery* m_EQSpawnEnemy = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float m_SpawnInterval = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	UCurveFloat* m_DifficultyCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float m_MaxEnemyNum = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<AActor> m_SpawnEnemy = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TObjectPtr<UDataTable> m_EnemySpawnTable;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AActor> m_SpawnItem[POWER_ITEM_NUM];

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	float m_CurEnemyNum = 0;

	UFUNCTION()
	void RunQuerySpwanEnemy();

	UFUNCTION()
	void RunQuerySpwanPowerItem();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnEnemyLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	UFUNCTION()
	void OnQueryPowerItemCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
};
