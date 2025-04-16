// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SInteractionComponent.h"
#include <Interfaces/SGamePlayInterface.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <UI/SUserWidgetWorld.h>
#include <../UnrealARPG.h>

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled()) {
		InteractBySweep();
	}
}

// 通过射线检测方式触发
void USInteractionComponent::InteractByLine()
{
	FCollisionObjectQueryParams CollisionParams;
	CollisionParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector EyeSeeEnd = EyeLocation + (EyeRotation.Vector() * 1000);
	FHitResult HitRes;
	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitRes, EyeLocation, EyeSeeEnd, CollisionParams);

	if (bHit)
	{
		AActor* HitActor = HitRes.GetActor();
		if (HitActor) {
			if (HitActor->Implements<USGamePlayInterface>()) {
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
			}
		}
	}
}

// 通过扫描检测方式触发
void USInteractionComponent::InteractBySweep()
{
	bool bDrawLine = CVarDrawDebugLine.GetValueOnGameThread();

	FCollisionObjectQueryParams CollisionParams;
	CollisionParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	APlayerCameraManager *CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (!CameraManager) {
		UE_LOG(LogTemp, Warning, TEXT("CameraManager is nullptr"));
		return;
	}

	FVector EyeLocation = CameraManager->GetCameraLocation();
	FRotator EyeRotation = CameraManager->GetCameraRotation();
	FVector EyeSeeEnd = EyeLocation + (EyeRotation.Vector() * m_MaxLineTraceLength);

	TArray<FHitResult> HitsArray;
	FCollisionShape CollisionShape;
	
	CollisionShape.SetSphere(m_SweepRadius);
	bool bHit = GetWorld()->SweepMultiByObjectType(HitsArray, EyeLocation, EyeSeeEnd, FQuat::Identity, CollisionParams, CollisionShape);
	FColor LineColor = bHit ? FColor::Green : FColor::Red;
	m_FocusActor = nullptr;

	if (bHit) {
		for (FHitResult HitRes : HitsArray) {
			AActor* HitActor = HitRes.GetActor();

			if (HitActor) {
				if (HitActor->Implements<USGamePlayInterface>()) {
					m_FocusActor = HitActor;

					if (bDrawLine) {
						DrawDebugSphere(GetWorld(), HitRes.ImpactPoint, m_SweepRadius, 30, LineColor, false, 0.02f);
					}
					
					break;
		   		}
			}
		}
	}

	if (bDrawLine) {
		DrawDebugLine(GetWorld(), EyeLocation, EyeSeeEnd, LineColor, false, 0.02f, 0, 2.0f);
	}

	if (m_FocusActor) {
		if (!m_ShowWidgetInstance) {
			m_ShowWidgetInstance = CreateWidget<USUserWidgetWorld>(GetWorld(), DefaultWidgetClass);
		}

		if (m_ShowWidgetInstance) {
			m_ShowWidgetInstance->m_AttachedActor = m_FocusActor;
			if (!m_ShowWidgetInstance->IsInViewport()) {
				m_ShowWidgetInstance->AddToViewport();
			}
		}
	} else {
		if (m_ShowWidgetInstance) {
			m_ShowWidgetInstance->RemoveFromParent();
			m_ShowWidgetInstance = nullptr;
		}
	}
}

void USInteractionComponent::PrimaryInteract()
{
	LogOnScreen(GetOwner(), FString("PrimaryInteract") + GetOwner()->GetName());
	ServerInteract(m_FocusActor);
}

//服务器运行函数，客户端会通过RPC调用
void USInteractionComponent::ServerInteract_Implementation(AActor* FocusActor)
{
	if (FocusActor == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("FocusActor is nullptr"));
		return;
	}

	LogOnScreen(GetOwner(), FString("Interact with ") + GetOwner()->GetName());
	ISGamePlayInterface::Execute_Interact(FocusActor, Cast<APawn>(GetOwner()));
}

