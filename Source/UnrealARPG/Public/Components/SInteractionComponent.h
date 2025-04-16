// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USUserWidgetWorld;

static TAutoConsoleVariable<bool> CVarDrawDebugLine(TEXT("pm.DrawLine"), false, TEXT("IsDrawDebugLine"), ECVF_Cheat);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALARPG_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void InteractByLine();
	void InteractBySweep();

public:	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	int32 m_MaxLineTraceLength = 500;

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float m_SweepRadius = 10.0f;

	UPROPERTY()
	AActor* m_FocusActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USUserWidgetWorld> DefaultWidgetClass;

	UPROPERTY()
	USUserWidgetWorld* m_ShowWidgetInstance = nullptr;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void PrimaryInteract();

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor *FocusActor);
};
