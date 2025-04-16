// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SGamePlayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class UNREALARPG_API ASItemChest : public AActor, public ISGamePlayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void Interact_Implementation(APawn* Target);

	void OnActorLoaded_Implementation();

protected:
	UPROPERTY(EditDefaultsOnly)
	float m_LidPitchVal = 90.f;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_BaseMeshCom;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* m_LidMeshCom;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly, SaveGame)
	bool m_bLidOpend = false;

	UFUNCTION()
	void OnRep_LidOpened();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

};
