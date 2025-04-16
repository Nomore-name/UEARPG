// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/SAction.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include <Action/SActionComponent.h>
#include <../UnrealARPG.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

void USAction::StartAction_Implementation(AActor* Instigator)
{
	LogOnScreen(Instigator, FString::Printf(TEXT("Start Action: %s"), *GetNameSafe(this)));

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority) {
		m_TimeStarted = GetWorld()->TimeSeconds;
	}

	USActionComponent* Comp = GetOwningComponent();
	if (Comp) {
		Comp->m_ActiveTags.AppendTags(m_GrantTags);
		m_RepData.bStarted = true;
		m_RepData.Instigator = Instigator;
		GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
	}
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	LogOnScreen(Instigator, FString::Printf(TEXT("Stop Action: %s"), *GetNameSafe(this)));

	USActionComponent* Comp = GetOwningComponent();
	if (Comp) {
		Comp->m_ActiveTags.RemoveTags(m_GrantTags);
		m_RepData.bStarted = false;
		m_RepData.Instigator = Instigator;
		GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
	}
}

bool USAction::IsStarted() const
{
	return m_RepData.bStarted;
}

bool USAction::CanStart_Implementation(AActor* Instigator) const
{
	if (m_RepData.bStarted) {
		 return false;
	}

	USActionComponent* Comp = GetOwningComponent();
	if (Comp) {
		return !Comp->m_ActiveTags.HasAny(m_BlockTags);
	}

	return true;
}

void USAction::OnRep_ChangeData()
{
	LogOnScreen(GetOuter(), "OnRep_ChangeData");

	if (m_RepData.bStarted) {
		StartAction(m_RepData.Instigator);
	} else {
		StopAction(m_RepData.Instigator);
	}
}

USActionComponent* USAction::GetOwningComponent() const
{
	// Outer 在使用 NewObject<T> 创建技能时传入
	//return Cast<USActionComponent>(GetOuter());
	return m_OwnerComp;
}

void USAction::InitializeComponent(USActionComponent* NewActionComp)
{
	m_OwnerComp = NewActionComp;
}

UWorld* USAction::GetWorld() const
{
	// Outer 在使用 NewObject<T> 创建技能时传入
	/*UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp) {
		return Comp->GetWorld();
	}

	return nullptr;*/

	if (m_OwnerComp) {
		return m_OwnerComp->GetWorld();
	}

	return nullptr;
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, m_RepData);
	DOREPLIFETIME(USAction, m_OwnerComp);
	DOREPLIFETIME(USAction, m_TimeStarted);
}

bool USAction::IsSupportedForNetworking() const
{
	return true;
}

