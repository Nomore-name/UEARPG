// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SAction.h"
#include "SProjectileAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnProjectileChanged, AActor*, InstigatorActor, int32, CurNum, int32, ChangeNum);
/**
 * 
 */
UENUM(BlueprintType)
enum class EProjectileType : uint8   
{
	E_TYPE_NORMAL,
	E_TYPE_CD,
	E_TYPE_ANGER,
};

UCLASS()
class UNREALARPG_API USProjectileAction : public USAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int32 GetCurNum() const;

	UFUNCTION(BlueprintCallable)
	EProjectileType GetProjectileType() const;

	UFUNCTION(BlueprintCallable)
	void SetIsReload(bool bReload);

	UFUNCTION()
	void ReloadProjectile(AActor* Instigator);

protected:
	UPROPERTY(EditDefaultsOnly, Replicated)
	bool m_bReload = false;
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<AActor> m_ProjectleClass = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	UAnimMontage* m_AttackAnim = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	FName m_SocketName = "Muzzle_01";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Attack")
	int32 m_ChangeNum = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_NumChange, Category = "Attack")
	int32 m_CurNum = 6;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category="Attack")
	int32 m_MaxNum = 6;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attack")
	float m_CDTime = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attack")
	float m_AngerNeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* m_MontageReload = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attack")
	EProjectileType m_ProjectileType = EProjectileType::E_TYPE_NORMAL;
	UPROPERTY(BlueprintAssignable)
	FOnProjectileChanged OnProjectileChange;

	FTimerHandle m_CDTimeHandle;

	void RegenerateProjectileNum();

	UFUNCTION()
	void ReloadProjectileNum(AActor* Instigator);
	
	UFUNCTION()
	void ReleaseProjectile(AActor* Instigator);

	UFUNCTION()
	bool StartCheck(AActor* Instigator);

	UFUNCTION()
	void OnRep_NumChange();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
