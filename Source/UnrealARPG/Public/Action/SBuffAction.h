// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SAction.h"
#include "SBuffAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBuffChanged, AActor*, InstigatorActor, int32, CurNum, int32, ChangeNum);
/**
 * 
 */
UENUM(BlueprintType)
enum class EBuffType : uint8   
{
	E_Buff,
	E_Debuff,
};


UCLASS()
class UNREALARPG_API USBuffAction : public USAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 m_CurNum = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 m_MaxNum = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float m_CDTime = 5.0f;

	UPROPERTY(BlueprintAssignable)
	FOnBuffChanged OnBuffChange;

	FTimerHandle m_TimerHandle;

	UFUNCTION()
	void RegenateBuffNum(AActor* Instigator);

	virtual void StartAction_Implementation(AActor* Instigator) override;
	
};
