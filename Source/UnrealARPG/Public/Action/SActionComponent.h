// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Runtime/GameplayTags/Classes/GameplayTagContainer.h>
#include "SActionComponent.generated.h"

class USAction;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, USActionComponent*, OwningComp, USAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALARPG_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USActionComponent();

public:	
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<USAction>> m_DefaultActions;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<TObjectPtr<USAction>> m_ArrayActions;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer m_ActiveTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsAciveTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	USAction* GetAction(FName ActionName) const;

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
