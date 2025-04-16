// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/SSaveGameSubsystem.h"
#include "Save/SDeveloperSaveSettings.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Save/SSaveGame.h"
#include "Runtime/Engine/Classes/GameFramework/GameStateBase.h"
#include "Runtime/CoreUObject/Public/Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Player/SPlayerState.h"
#include "Interfaces/SGamePlayInterface.h"

void USSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USDeveloperSaveSettings* SavaSetting = GetDefault<USDeveloperSaveSettings>();
	m_CurSaveSlotName = SavaSetting->SaveSlotName;

	UDataTable* DataTable = SavaSetting->DummyTablePath.LoadSynchronous();
}

void USSaveGameSubsystem::SetSaveSlotName(FString NewSlotName)
{
	if (NewSlotName.Len() <= 0) {
		return;
	}

	m_CurSaveSlotName = NewSlotName;
}

void USSaveGameSubsystem::WriteSaveGame()
{
	m_SaveGame->m_SavedPlayers.Empty();
	m_SaveGame->m_SaveActorData.Empty();

	AGameStateBase* GS = GetWorld()->GetGameState();
	if (!GS) {
		return;
	}

	for (int32 i = 0; i < GS->PlayerArray.Num(); ++i) {
		ASPlayerState* PS = Cast<ASPlayerState>(GS->PlayerArray[i]);
		if (PS) {
			PS->SavePlayerState(m_SaveGame);
			break;
		}
	}

	// 遍历世界上的所有 Actor
	for (FActorIterator it(GetWorld()); it; ++it) {
		AActor* Actor = *it;
		if (!Actor->Implements<USGamePlayInterface>()) {
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
        ActorData.Transform = Actor->GetActorTransform();
		
		//FMemoryWriter对象，它是一个用于写入数据到内存的类，这里将数据写入ActorData.ByteData中
		FMemoryWriter MemWriter(ActorData.ByteData);

		//一个用于序列化的类，这里使用MemWriter作为输出目标，意味着所有通过Ar归档的数据都会被写入到MemWriter指定的内存位置
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);

		// 为ture则只存储标记了UPROPERTY(SaveGame)的变量
        Ar.ArIsSaveGame = true;

		//调用AActor对象的Serialize函数，并传入Ar归档器。Serialize函数会处理对象的状态信息的序列化
        Actor->Serialize(Ar);

		m_SaveGame->m_SaveActorData.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(m_SaveGame, m_CurSaveSlotName, 0);
	OnSaveGameWritten.Broadcast(m_SaveGame);
}

void USSaveGameSubsystem::LoadSaveGame(FString SaveSlotName /*= ""*/)
{
	SetSaveSlotName(SaveSlotName);

	if (UGameplayStatics::DoesSaveGameExist(m_CurSaveSlotName, 0)) {
		m_SaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(m_CurSaveSlotName, 0));
		if (!m_SaveGame) {
			UE_LOG(LogTemp, Log, TEXT("No SaveGame"));
			return;
		}

		for (FActorIterator It(GetWorld()); It; ++It) {
			AActor* Actor = *It;
			if (!Actor->Implements<USGamePlayInterface>()) {
				continue;
			}

			for (FActorSaveData& ActorData : m_SaveGame->m_SaveActorData) {
				if (ActorData.ActorName == Actor->GetName()) {
					Actor->SetActorTransform(ActorData.Transform);

					//将存储的序列化信息读入Actor
					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);

					ISGamePlayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}

		OnSaveGameLoaded.Broadcast(m_SaveGame);
	} else {
		m_SaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame"));
	}
}

void USSaveGameSubsystem::RestorPlayerData(AController* NewPlayer)
{
	if (!NewPlayer) {
		return;
	}

	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS) {
		PS->LoadPlayerState(m_SaveGame);
	}
}

bool USSaveGameSubsystem::RestorTransformData(AController* NewPlayer)
{
	if (!NewPlayer || (NewPlayer && NewPlayer->IsPendingKill())) {
		return false;
	}

	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (!PS) {
		return false;
	}

	APawn* Pawn = PS->GetPawn();
	if (!Pawn) {
		return false;
	}

	FPlayerSaveData* FoundData = m_SaveGame->GetPlayerData(PS);
	if (FoundData && FoundData->bResumeAtTransform) {
		Pawn->SetActorLocation(FoundData->Location);
		Pawn->SetActorRotation(FoundData->Rotation);

		//PlayerState owner 就是Player Controller
		AController* PC = Cast<AController>(PS->GetOwner());
		//设置控制旋转来改变相机方向，单单设置Pawn旋转是不够的
		PC->SetControlRotation(FoundData->Rotation);
		return true;
	}

	return false;
}
