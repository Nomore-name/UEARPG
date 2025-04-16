// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SPowerItemBase.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

// Sets default values
ASPowerItemBase::ASPowerItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = m_SphereComp;

	bReplicates = true;
}

void ASPowerItemBase::HideAndCooldownItem()
{
	SetPowerItemVisible(false);

	FTimerHandle TimeHandleRespawn;
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ASPowerItemBase::SetPowerItemVisible, true);

	GetWorldTimerManager().SetTimer(TimeHandleRespawn, RespawnDelegate, m_RespawnTime, false);
}

void ASPowerItemBase::SetPowerItemVisible(bool bVisible)
{
	SetActorEnableCollision(bVisible);
	//第二个参数为true代表影响子类
	RootComponent->SetVisibility(bVisible, true);
}

// Called when the game starts or when spawned
void ASPowerItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPowerItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPowerItemBase::Interact_Implementation(APawn* Target)
{

}

