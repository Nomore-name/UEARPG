// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SSaveGameSubsystem.generated.h"

class USSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignature, class USSaveGame*, SaveObject);

/**
 * 
 */
UCLASS(meta=(DisplayName="SaveGameSystem")) 
class UNREALARPG_API USSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	FString m_CurSaveSlotName;

	UPROPERTY()
	USSaveGame* m_SaveGame;

public:
	// 初始化子系统
	void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void SetSaveSlotName(FString NewSlotName);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame(FString SaveSlotName = "");

	void RestorPlayerData(AController* NewPlayer);

	UFUNCTION(BlueprintCallable)
	bool RestorTransformData(AController* NewPlayer);

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameLoaded;

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameWritten;
};
