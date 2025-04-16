// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/SActionComponent.h"
#include "Action/SAction.h"
#include <../UnrealARPG.h>
#include <Runtime/Engine/Classes/Engine/ActorChannel.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	//仅加载在服务器,通过m_mapActions复制到客户端
	if (GetOwner()->HasAuthority()) {
		for (TSubclassOf<USAction> Action : m_DefaultActions) {
			AddAction(GetOwner(), Action);
		}
	}
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ActionClass) {
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);

	if (NewAction) {
		NewAction->InitializeComponent(this);
		m_ArrayActions.Add(NewAction);
		if (NewAction->m_bAutoStart && NewAction->CanStart(Instigator)) {
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ActionToRemove || (ActionToRemove && !ActionToRemove->IsStarted())) {
		return;
	}

	m_ArrayActions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : m_ArrayActions) {
		if (Action && Action->m_ActionName == ActionName) {

			if (!Action->CanStart(Instigator)) {
				LogOnScreen(Instigator, FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString()), FColor::Red);
				return false;
			}

			// 如果是客户端，则调用服务器端的StartAction函数
			if (!GetOwner()->HasAuthority()) {
				ServerStartAction(Instigator, ActionName);
			}

			Action->StartAction(Instigator);
			return true;
		}
	}
	
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : m_ArrayActions) {
		if (Action && Action->m_ActionName == ActionName) {
			if (Action->IsStarted()) {
				
				if (!GetOwner()->HasAuthority()) {
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

bool USActionComponent::IsAciveTag(FGameplayTag Tag)
{
	return m_ActiveTags.HasTag(Tag);
}

USAction* USActionComponent::GetAction(FName ActionName) const
{
	for (USAction* Action : m_ArrayActions) {
		if (Action && Action->m_ActionName == ActionName) {
			return Action;
		}
	}

	return nullptr;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

//由于UObject派生类SAction并不支持网络复制，需要重写ReplicateSubobjects来手动实现
bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (USAction* Action : m_ArrayActions) {
		bWroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
	}

	return bWroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, m_ArrayActions);
}

