// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SAttributeComponent.h"
#include <Runtime/Experimental/ChaosCore/Public/Chaos/Vector.h>
#include <Game/SGameModeBase.h>
#include <../UnrealARPG.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("pm.DamageMultipler"), 1.0f, TEXT("Damage"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	m_Health = m_MaxHealth;
	
	SetIsReplicatedByDefault(true);
}

USAttributeComponent* USAttributeComponent::GetAttributeComponent(AActor* Actor)
{
	if (Actor) {
		return Cast<USAttributeComponent>(Actor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool USAttributeComponent::AddHealth(AActor *InstigatorActor, float Value)
{
	// 只在服务端运行伤害计算
	if (!GetOwner()->HasAuthority()) {
		return false;
	}

	if (Value < 0.0f) {
		Value *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	float OldHealth = m_Health;

	m_Health = FMath::Clamp(m_Health + Value, 0.0f, m_MaxHealth);				//Clamp限制范围在0.0f~m_MaxHealth之间
	//OnHealthChanged.Broadcast(InstigatorActor, this, m_Health, Value);
    MulitcastHealthChanged(InstigatorActor, m_Health, Value);

	if (m_Health <= 0 && Value <= 0 && OldHealth != 0) {

		ASGameModeBase *GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		
		if (GM) {
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return OldHealth != m_Health;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return AddHealth(InstigatorActor, -m_Health);
}

bool USAttributeComponent::IsAlive() const
{
	return m_Health > 0;
}

bool USAttributeComponent::IsMaxHealth() const
{
	return m_Health >= m_MaxHealth;
}

float USAttributeComponent::GetMaxHealth() const
{
	return m_MaxHealth;
}

float USAttributeComponent::GetCurHealth() const
{
	return m_Health;
}

bool USAttributeComponent::AddAnger(AActor* InstigatorActor, float Value)
{
	if (!GetOwner()->HasAuthority()) {
		return false;
	}

	float OldAnger = m_Anger;
	m_Anger = FMath::Clamp(m_Anger + Value, 0.0f, m_MaxAnger);
	//OnAngerChanged.Broadcast(InstigatorActor, this, m_Anger, Value);
	ClientAngerChanged(InstigatorActor, m_Anger, Value);

	return OldAnger != m_Anger;
}

bool USAttributeComponent::IsMaxAnger() const
{
	return m_Anger >= m_MaxAnger;
}

float USAttributeComponent::GetMaxAnger() const
{
	return m_MaxAnger;
}

float USAttributeComponent::GetCurAnger() const
{
	return m_Anger;
}

//NetMulticast RPC，服务器调用时：不管Actor所有权都会在服务器和所有客户端上运行
void USAttributeComponent::MulitcastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float ChangeValue)
{
	//LogOnScreen(GetOwner(), FString::Printf(TEXT("Health Changed: %f "), NewHealth) + GetOwner()->GetName());
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, ChangeValue);
}

//Client RPC，服务器调用时：Client-owned actor会在actor的所属客户端上运行, Server-owned actor会在在服务器上运行
void USAttributeComponent::ClientAngerChanged_Implementation(AActor* InstigatorActor, float NewAnger, float ChangeValue)
{
	//LogOnScreen(GetOwner(), FString::Printf(TEXT("Anger Changed: %f "), NewAnger) + GetOwner()->GetName());
	OnAngerChanged.Broadcast(InstigatorActor, this, NewAnger, ChangeValue);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, m_Health);
	DOREPLIFETIME(USAttributeComponent, m_MaxHealth);
	DOREPLIFETIME(USAttributeComponent, m_Anger);
	DOREPLIFETIME(USAttributeComponent, m_MaxAnger);
}

