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
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// BlueprintNativeEvent = C++基础实现，在蓝图中可以拓展，使用BlueprintCallable允许蓝图子类调用函数来触发该函数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	//Actor在构造函数后，PostInitializeComponents将被调用，通常在这绑定相关方法，在构造函数中容易被覆生成的蓝图盖掉
	virtual void PostInitializeComponents() override;

	virtual void DestorySelf();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	float m_ImpactShakeInnerRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float m_ImpactShakeOuterRadius;

private:
	FTimerHandle m_DestorySelfTimeHandle;

};
