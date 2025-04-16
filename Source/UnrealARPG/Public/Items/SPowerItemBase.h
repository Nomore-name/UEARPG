// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SGamePlayInterface.h"
#include "SPowerItemBase.generated.h"

class USphereComponent;

UCLASS()
class UNREALARPG_API ASPowerItemBase : public AActor, public ISGamePlayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerItemBase();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Compoments")
	USphereComponent* m_SphereComp = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float m_RespawnTime = 5.0f;

	void HideAndCooldownItem();

	void SetPowerItemVisible(bool bVisible);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* Target) override;

};
