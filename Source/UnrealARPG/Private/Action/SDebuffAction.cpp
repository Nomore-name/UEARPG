// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/SDebuffAction.h"
#include <Action/SActionComponent.h>
#include "Runtime/Engine/Classes/GameFramework/GameStateBase.h"

USDebuffAction::USDebuffAction()
{
	m_bAutoStart = true;
}

float USDebuffAction::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS) {
		float EndTime = m_TimeStarted + m_Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}
	return m_Duration;
}

void USDebuffAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (m_Duration > 0.0f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(m_EffectTimeHandle, Delegate, m_Duration, false);
	}

	if (m_Period > 0.0f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(m_PeriodTimeHandle, Delegate, m_Period, true);
	}
}

void USDebuffAction::StopAction_Implementation(AActor* Instigator)
{
	//检查周期效果是否会同时触发，如果时间近乎一样，那么先触发周期效果，再停止Action
	if (m_Period > 0.0f && GetWorld()->GetTimerManager().GetTimerRemaining(m_PeriodTimeHandle) < KINDA_SMALL_NUMBER) {
		ExecutePeriodEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(m_EffectTimeHandle);
	GetWorld()->GetTimerManager().ClearTimer(m_PeriodTimeHandle);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp) {
		Comp->RemoveAction(this);
	}
}

void USDebuffAction::ExecutePeriodEffect_Implementation(AActor* Instigator)
{

}