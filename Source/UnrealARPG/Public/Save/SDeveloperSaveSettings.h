// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SDeveloperSaveSettings.generated.h"

/**
 * 
 */
 // 'defaultconfig' = 只保存对象配置到default.ini，不保存到local.ini。
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Save Game Settings"))
class UNREALARPG_API USDeveloperSaveSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	USDeveloperSaveSettings();

	/* Default slot name if UI doesn't specify any */ 
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	FString SaveSlotName;

	/* Soft path must be converted to asset before use */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> DummyTablePath;
};
