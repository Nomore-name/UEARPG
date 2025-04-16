// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/SBuffAction.h"
#include "Runtime/Engine/Classes/GameFramework/GameStateBase.h"

void USBuffAction::StartAction_Implementation(AActor* Instigator)
{
	if (m_CurNum <= 0) {
		return;
	}

	Super::StartAction_Implementation(Instigator);
	
	int32 OldNum = m_CurNum;
	m_CurNum = FMath::Clamp(m_CurNum - 1, 0, m_MaxNum);
	OnBuffChange.Broadcast(Instigator, m_CurNum, m_CurNum - OldNum);

	if (!Instigator->GetWorldTimerManager().IsTimerActive(m_TimerHandle)) {
		FTimerDelegate TimeDelegate;
		TimeDelegate.BindUFunction(this, "RegenateBuffNum", Instigator);
		Instigator->GetWorldTimerManager().SetTimer(m_TimerHandle, TimeDelegate, m_CDTime, true);
	}
}

float USBuffAction::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS) {
		float EndTime = m_TimeStarted + m_CDTime;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}
	return m_CDTime;
}

void USBuffAction::RegenateBuffNum(AActor* Instigator)
{
	int32 OldNum = m_CurNum;
	m_CurNum = FMath::Clamp(m_CurNum + 1, 0, m_MaxNum);
	OnBuffChange.Broadcast(Instigator, m_CurNum, m_CurNum - OldNum);

	if (m_CurNum == m_MaxNum) {
		Instigator->GetWorldTimerManager().ClearTimer(m_TimerHandle);
	}

	StopAction_Implementation(Instigator);
}

