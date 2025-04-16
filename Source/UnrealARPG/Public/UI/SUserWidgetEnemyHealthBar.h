// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SUserWidgetEnemyHealthBar.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class UNREALARPG_API USUserWidgetEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))		//将该属性标记为一个可以与用户界面蓝图中的控件绑定的属性
	USizeBox *m_ParentSizeBox;

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector m_WorldOffset;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	AActor *m_AttachedActor;
};
