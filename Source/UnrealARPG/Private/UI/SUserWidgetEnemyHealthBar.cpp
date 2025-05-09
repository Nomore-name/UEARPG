// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SUserWidgetEnemyHealthBar.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/UMG/Public/Components/Widget.h>
#include <Runtime/UMG/Public/Components/SizeBox.h>
#include <Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>

void USUserWidgetEnemyHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!m_AttachedActor) {
		RemoveFromParent();
		return;
	}

	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), m_AttachedActor->GetActorLocation() + m_WorldOffset, ScreenPosition)) {
		
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;
		if (m_ParentSizeBox) {
			m_ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
