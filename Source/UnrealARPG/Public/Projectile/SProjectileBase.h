// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class USoundCue;
class UCameraShakeBase;

UCLASS(ABSTRACT)			// 'ABSTRACT' 将这个类标记为未完成,使用这个标记可以让这个类不在虚幻编辑器的类如SpawnActor的下拉窗口中出现
class UNREALARPG_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// BlueprintNativeEvent = C++基础实现，在蓝图中可以拓展，使用BlueprintCallable允许蓝图子类调用函数来触发该函数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	UFUNCTION()
	void OnRep_Used();

	//Actor在构造函数后，PostInitializeComponents将被调用，通常在这绑定相关方法，在构造函数中容易被覆生成的蓝图盖掉
	virtual void PostInitializeComponents() override;

	virtual void HideSelf();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//使用ExposeOnSpawn = true时，必须要EditAnywhere和BlueprintReadWrite，否则会编译出错
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool", Meta = (ExposeOnSpawn = true))
	float m_AliveTime = 5.0f;

    UFUNCTION(BlueprintCallable, Category = "Pool")
	virtual void SetIsUse(bool bUse);

	bool IsUsed() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* m_SphereCom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UProjectileMovementComponent* m_PjMoveCom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* m_ParticleSysCom;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* m_AudioComp;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* m_ImpactSound;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* m_EmittParticle;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase>  m_ImpactShake;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float m_ImpactShakeInnerRadius = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float m_ImpactShakeOuterRadius = 1500.0f;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_Used, Category = "Pool")
	bool m_bUsed = false;

private:
	FTimerHandle m_HideTimeHandle;

};
