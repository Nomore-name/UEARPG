// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SItemChest.h"
#include "Runtime/Engine/Public/Net/UnrealNetwork.h"
#include <../UnrealARPG.h>

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_BaseMeshCom = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshCom");
	RootComponent = m_BaseMeshCom;
	m_LidMeshCom = CreateDefaultSubobject<UStaticMeshComponent>("LidMeshCom");
	m_LidMeshCom->SetupAttachment(m_BaseMeshCom);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

//因为InteractionComponent里交互（ServerInteract）为服务器上运行，所以该函数也就只会在服务器上运行
void ASItemChest::Interact_Implementation(APawn* Target)
{
	if (!m_LidMeshCom) {
		return;
	}

	m_bLidOpend = !m_bLidOpend;
	OnRep_LidOpened();		//监听服务器不会接收到ReplicatedUsing，所以需要手动调用
}

void ASItemChest::OnRep_LidOpened()
{
	LogOnScreen(this, "ASItemChest OnRep_LidOpened");
	if (m_bLidOpend) {
		m_LidMeshCom->SetRelativeRotation(FRotator(0, 0, m_LidPitchVal));
	} else {
		m_LidMeshCom->SetRelativeRotation(FRotator(0, 0, 0));
	}
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, m_bLidOpend);
}

