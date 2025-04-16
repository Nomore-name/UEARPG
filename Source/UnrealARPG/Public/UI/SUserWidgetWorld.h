// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SUserWidgetWorld.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class UNREALARPG_API USUserWidgetWorld : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> m_ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector m_WorldOffset;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn=true))
	TObjectPtr<AActor> m_AttachedActor;
};
