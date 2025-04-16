// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SShield.generated.h"

class UStaticMeshComponent;
class USAttributeComponent;

UCLASS()
class UNREALARPG_API ASShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_BaseMeshCom = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	USAttributeComponent* m_AttributeComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float m_RegRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	float m_RegValue = 10.f;

	FTimerHandle m_TimeHandleRegShield;

	UFUNCTION()
	void RegenateShield();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetShieldCollision(bool bOpen);

	UFUNCTION()
	void SetShieldRegenate(bool bStart);

	UFUNCTION()
	USAttributeComponent* GetAttributeComp() const;
};
