// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SUserWidgetWorld.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include <Runtime/UMG/Public/Components/Widget.h>
#include <Runtime/UMG/Public/Components/SizeBox.h>

void USUserWidgetWorld::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(m_AttachedActor)) {
		RemoveFromParent();
		return;
	}

	FVector2D ScreenPosition;
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), m_AttachedActor->GetActorLocation() + m_WorldOffset, ScreenPosition);

	if (bIsOnScreen) {
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale;

		if (m_ParentSizeBox) {
			m_ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

	if (m_ParentSizeBox) {
		m_ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
