// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScoreChanged, int32, ChangeScore, int32, NewScore, bool, bRealChange);
/**
 * 
 */
UCLASS()
class UNREALARPG_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	int32 m_Score = 0;

public:
	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveGame);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable)
	void IncreaseScore(int32 ChangeScore);

	UFUNCTION(BlueprintCallable)
	void UnSucScoreChange(int32 ChangeScore);

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerScore();

	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;
};
