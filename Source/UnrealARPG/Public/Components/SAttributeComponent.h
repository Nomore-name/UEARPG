// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwingComp, float, NewHealth, float, ChangeValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAngerChanged, AActor*, InstigatorActor, USAttributeComponent*, OwingComp, float, NewAnger, float, ChangeValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALARPG_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	static USAttributeComponent* GetAttributeComponent(AActor* Actor);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool AddHealth(AActor* InstigatorActor, float Value);

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Anger")
	bool AddAnger(AActor* InstigatorActor, float Value);

	UFUNCTION(BlueprintCallable, Category = "Anger")
	bool IsMaxAnger() const;

	UFUNCTION(BlueprintCallable, Category = "Anger")
	float GetMaxAnger() const;

	UFUNCTION(BlueprintCallable, Category = "Anger")
	float GetCurAnger() const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAngerChanged OnAngerChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float m_Anger = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float m_MaxAnger = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float m_Health = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float m_MaxHealth = 100.f;

	UFUNCTION(NetMulticast, Reliable)
	void MulitcastHealthChanged(AActor* InstigatorActor, float NewHealth, float ChangeValue);

    UFUNCTION(Client, Reliable)
	void ClientAngerChanged(AActor* InstigatorActor, float NewAnger, float ChangeValue);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
