// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Runtime/GameplayTags/Classes/GameplayTagContainer.h>
#include "SAction.generated.h"

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bStarted = false;

	UPROPERTY()
	AActor* Instigator = nullptr;
};

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALARPG_API USAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	FName m_ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool m_bAutoStart = false;

	UPROPERTY(Replicated)
	float m_TimeStarted;

	UPROPERTY(Replicated)
	USActionComponent* m_OwnerComp = nullptr;

	UFUNCTION(BlueprintNativeEvent)
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	bool IsStarted() const;

	UFUNCTION(BluePrintNativeEvent)
	bool CanStart(AActor* Instigator) const;

	UFUNCTION()
	void OnRep_ChangeData();

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	UFUNCTION()
	void InitializeComponent(USActionComponent* NewActionComp);

	virtual UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing="OnRep_ChangeData")
	FActionRepData m_RepData;

	//授予执行的标签
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer m_GrantTags;

    //当前标签执行时，被阻止执行的标签
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer m_BlockTags;
};
