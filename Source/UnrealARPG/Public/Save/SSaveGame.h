// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"


USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
    FString ActorName;

	UPROPERTY()
    FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:

	FPlayerSaveData()
	{
		Score = 0;
		PersonalRecordTime = 0.0f;
		Location = FVector::ZeroVector;
		Rotation = FRotator::ZeroRotator;
		bResumeAtTransform = true;
	}

	UPROPERTY()
	FString PlayerID;

	UPROPERTY()
    int32 Score;

	UPROPERTY()
    float PersonalRecordTime;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	bool bResumeAtTransform;
};

/**
 * 
 */
UCLASS()
class UNREALARPG_API USSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FActorSaveData> m_SaveActorData;

	UPROPERTY()
	TArray<FPlayerSaveData> m_SavedPlayers;

public:
	FPlayerSaveData* GetPlayerData(APlayerState* PlayerState);
};
