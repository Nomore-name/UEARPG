// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/SProjectileAction.h"
#include "Character/SCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include <Components/SInteractionComponent.h>
#include "Action/SActionComponent.h"
#include <Components/SAttributeComponent.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../UnrealARPG.h>

bool USProjectileAction::StartCheck(AActor* Instigator)
{
	switch (m_ProjectileType)
	{
	case EProjectileType::E_TYPE_NORMAL:
		{
			if (m_CurNum <= 0) {
				m_bReload = true;
				//ReloadProjectile(Instigator);
				return true;
			}
		}
		break;
	case EProjectileType::E_TYPE_CD:
		{
			if (m_CurNum <= 0) {
				return false;
			}
		}
		break;
	case EProjectileType::E_TYPE_ANGER:
		{
			USAttributeComponent* AttrComp = Cast<USAttributeComponent>(Instigator->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (AttrComp && AttrComp->GetCurAnger() >= m_AngerNeed) {
				return true;
			}else {
				return false;
			}
		}
		break;
	default:
		return false;
		break;
	}

	return true;
}

void USProjectileAction::OnRep_NumChange()
{
	USActionComponent* Comp = GetOwningComponent();
	if (Comp) {
		OnProjectileChange.Broadcast(Comp->GetOwner(), m_CurNum, m_ChangeNum);
	}
}

void USProjectileAction::StartAction_Implementation(AActor* Instigator)
{
	if (!StartCheck(Instigator)) {
		return;
	}

	if (m_bReload) {
		ReloadProjectile(Instigator);
		return;
	}

	Super::StartAction_Implementation(Instigator);

	ASCharacter* Character = Cast<ASCharacter>(Instigator);
	if (Character) {
		Character->PlayAnimMontage(m_AttackAnim);

		//客户端只负责表现，将生成子弹的逻辑放在服务器
		if (Character->HasAuthority()) {
			FTimerHandle TimerHandle;
			FTimerDelegate TimeDelegate;
			TimeDelegate.BindUFunction(this, "ReleaseProjectile", Instigator);
			Character->GetWorldTimerManager().SetTimer(TimerHandle, TimeDelegate, 0.2f, false);
		}
	}
}

int32 USProjectileAction::GetCurNum() const
{
	return m_CurNum;
}

EProjectileType USProjectileAction::GetProjectileType() const
{
	return m_ProjectileType;
}

void USProjectileAction::RegenerateProjectileNum()
{
	if (m_CurNum >= m_MaxNum) {
		GetWorld()->GetTimerManager().ClearTimer(m_CDTimeHandle);
		return;
	}

	m_ChangeNum = 1;
	m_CurNum += m_ChangeNum;
	OnRep_NumChange();

	/*USActionComponent* Comp = GetOwningComponent();
	if (Comp) {
		OnProjectileChange.Broadcast(Comp->GetOwner(), m_CurNum, 1);
	}*/
}

void USProjectileAction::SetIsReload(bool bReload)
{
	m_bReload = bReload;
}

void USProjectileAction::ReloadProjectile(AActor* Instigator)
{
	if (!m_MontageReload || !Instigator) {
		UE_LOG(LogTemp, Log, TEXT("m_MontageReload or Instigator is nullptr"));
		return;
	}

	Super::StartAction_Implementation(Instigator);

	USActionComponent* Comp = GetOwningComponent();
	if (!Comp) {
		return;
	}

	ASCharacter* Character = Cast<ASCharacter>(Comp->GetOwner());
	if (!Character) {
		return;
	}

	float AniTime = Character->PlayAnimMontage(m_MontageReload);
	if (Character->HasAuthority()) {
		FTimerHandle TimeHandReload;
		FTimerDelegate TimeDelegate;
		TimeDelegate.BindUFunction(this, "ReloadProjectileNum", Instigator);
		GetWorld()->GetTimerManager().SetTimer(TimeHandReload, TimeDelegate, AniTime, false);
	}
}

void USProjectileAction::ReloadProjectileNum(AActor* Instigator)
{
	m_bReload = false;
	m_ChangeNum = m_MaxNum > m_CurNum ? m_MaxNum - m_CurNum : 0;
	m_CurNum += m_ChangeNum;
	OnRep_NumChange();

	/*USActionComponent* Comp = GetOwningComponent();
	if (Comp) {
		OnProjectileChange.Broadcast(Comp->GetOwner(), m_CurNum, m_MaxNum);
	}*/

	Super::StopAction_Implementation(Instigator);
}

void USProjectileAction::ReleaseProjectile(AActor* Instigator)
{
	ASCharacter* Character = Cast<ASCharacter>(Instigator);

	if (!Character) {
		return;
	}

	const UCameraComponent* CameraComp = Character->GetPlayerCamera();
	FVector CameraSeeEnd = CameraComp->GetComponentLocation() + (CameraComp->GetComponentRotation().Vector() * 10000);
	TArray<FHitResult> HitsArray;
	FCollisionShape CollisionShape;
	FCollisionObjectQueryParams CollisionParams;

	CollisionShape.SetSphere(10.f);
	CollisionParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	CollisionParams.AddObjectTypesToQuery(ECC_WorldStatic);

	bool bHit = GetWorld()->SweepMultiByObjectType(HitsArray, CameraComp->GetComponentLocation(), CameraSeeEnd, FQuat::Identity, CollisionParams, CollisionShape);

	FVector HitLocation = bHit ? HitsArray[0].ImpactPoint : CameraSeeEnd;
	FVector HandLocation = Character->GetMesh()->GetSocketLocation(m_SocketName);
	FRotator HitRotator = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitLocation);
	FTransform SpawnTrans = FTransform(HitRotator, HandLocation);

	/*if (bHit) {
		UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *HitLocation.ToString());
	} else {
		UE_LOG(LogTemp, Log, TEXT("No Hit: %s"), *HitLocation.ToString());
	}*/

	//配置在生成新的Actor时的各种参数
	FActorSpawnParameters SpawnParams;
	//SpawnCollisionHandlingOverride用于指定引擎在生成新Actor时应该如何处理碰撞,AlwaysSpawn表示总生成新的Actor,即使它可能会与现有的Actor发生重叠
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Character;

	Character->GetWorld()->SpawnActor<AActor>(m_ProjectleClass, SpawnTrans, SpawnParams);

	m_ChangeNum = -1;
	m_CurNum += m_ChangeNum;
	OnRep_NumChange();

	/*USActionComponent* Comp = GetOwningComponent();
	if (Comp) {
		OnProjectileChange.Broadcast(Comp->GetOwner(), m_CurNum, -1);
	}*/

	if (m_CurNum <= 0 && m_ProjectileType == EProjectileType::E_TYPE_CD) {
		GetWorld()->GetTimerManager().ClearTimer(m_CDTimeHandle);
		GetWorld()->GetTimerManager().SetTimer(m_CDTimeHandle, this, &USProjectileAction::RegenerateProjectileNum, m_CDTime, true);
	}

	if (m_ProjectileType == EProjectileType::E_TYPE_ANGER) {
		USAttributeComponent* AttrComp = Cast<USAttributeComponent>(Instigator->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttrComp) {
			AttrComp->AddAnger(Instigator, -m_AngerNeed);
		}
	}

	if (CVarDrawDebugLine.GetValueOnGameThread()) {
		FColor LineColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), CameraComp->GetComponentLocation(), HitLocation, LineColor, false, 2.0f, 0, 2.0f);
	}

	StopAction_Implementation(Instigator);
}

void USProjectileAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(USProjectileAction, m_CurNum, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USProjectileAction, m_MaxNum, COND_OwnerOnly);
	DOREPLIFETIME(USProjectileAction, m_bReload);
}
