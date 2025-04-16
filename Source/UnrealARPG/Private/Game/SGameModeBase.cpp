// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameModeBase.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/AIModule/Classes/EnvironmentQuery/EnvQueryTypes.h"
#include "Runtime/AIModule/Classes/EnvironmentQuery/EnvQueryManager.h"
#include "Runtime/AIModule/Classes/EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/CoreUObject/Public/UObject/ReflectedTypeAccessors.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/CoreUObject/Public/Serialization/ObjectAndNameAsStringProxyArchive.h>
#include <Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Action/SActionComponent.h"
#include "Save/SSaveGameSubsystem.h"
#include "Components/SAttributeComponent.h"
#include "Datas/SEnemyDataAsset.h"
#include "Player/SPlayerState.h"
#include "Character/SCharacter.h"
#include "../UnrealARPG.h"

static TAutoConsoleVariable<bool> CVarBotSpawn(TEXT("pm.BotSpawn"), true, TEXT("BotSpawn"), ECVF_Cheat);

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	RunQuerySpwanEnemy();
	RunQuerySpwanPowerItem();

	FTimerHandle TimeHandleSpawn;
	GetWorldTimerManager().SetTimer(TimeHandleSpawn, this, &ASGameModeBase::RunQuerySpwanEnemy, m_SpawnInterval, true);
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	USSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<USSaveGameSubsystem>();
	//可选的插槽存档名称，没有的话返回到savegamessettings类/INI中指定的插槽名
	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	SG->LoadSaveGame(SelectedSaveSlot);
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	USSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<USSaveGameSubsystem>();
	if (SG) {
		SG->RestorPlayerData(NewPlayer);
		SG->RestorTransformData(NewPlayer);
	}
	
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ACharacter> it(GetWorld()); it; ++it)
	{
		ACharacter* Character = *it;

		if (Character && Character->ActorHasTag(FName("Enemy"))) {

			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Character->GetComponentByClass(USAttributeComponent::StaticClass()));

			if (AttributeComp) {
				AttributeComp->Kill(this);
			}
		}
	}
}

void ASGameModeBase::RunQuerySpwanEnemy()
{
	if (!CVarBotSpawn.GetValueOnGameThread()) {
		return;
	}

	m_CurEnemyNum = 0;

	for (TActorIterator<ACharacter> it(GetWorld()); it; ++it)
	{
		ACharacter* Character = *it;

		if (Character && Character->ActorHasTag(FName("Enemy"))) {

			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Character->GetComponentByClass(USAttributeComponent::StaticClass()));

			if (AttributeComp && AttributeComp->IsAlive()) {
				m_CurEnemyNum++;
			}
		}
	}

	if (m_DifficultyCurve) {
		m_MaxEnemyNum = m_DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (m_MaxEnemyNum <= m_CurEnemyNum) {
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* EQInstance = UEnvQueryManager::RunEQSQuery(this, m_EQSpawnEnemy, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (EQInstance) {
		EQInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::RunQuerySpwanPowerItem()
{
	UEnvQueryInstanceBlueprintWrapper* EQInstance = UEnvQueryManager::RunEQSQuery(this, m_EQSpowerItem, this, EEnvQueryRunMode::AllMatching, nullptr);

	if (EQInstance) {
		EQInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryPowerItemCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Enemy EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0)) {
		if (m_EnemySpawnTable) {
			TArray<FEnemyInfoRow*> Rows;
			m_EnemySpawnTable->GetAllRows("", Rows);

			float TotalWeight = 0.f;
			for (const FEnemyInfoRow* Row : Rows) {
				TotalWeight += Row->SpwanWight;
			}

			if (TotalWeight <= 0) {
				UE_LOG(LogTemp, Warning, TEXT("敌人权重都为0，不生成敌人！"));
				return;
			}
			
			// 根据权重随机选择敌人
			float RandomWeight = FMath::RandRange(0.f, TotalWeight);
			float CurrentWeight = 0.0f;
			int32 SelectedIndex = -1;
			for (int32 i = 0; i < Rows.Num(); ++i) {
				CurrentWeight += Rows[i]->SpwanWight;
				if (RandomWeight <= CurrentWeight) {
					SelectedIndex = i;
					break;
				}
			}

			if (SelectedIndex == -1) {
				return;
			}

			UAssetManager* Manger = UAssetManager::GetIfValid();
			if (Manger) {
				LogOnScreen(this, "Enemy Load");
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnEnemyLoaded, Rows[SelectedIndex]->EnemyID, Locations[0]);
				Manger->LoadPrimaryAsset(Rows[SelectedIndex]->EnemyID, Bundles, Delegate);
			}
		}
	}
}

void ASGameModeBase::OnEnemyLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Enemy Loaded Finish");

	UAssetManager* Manger = UAssetManager::GetIfValid();
	if (!Manger) {
		return;
	}

	USEnemyDataAsset* EnemyData = Cast<USEnemyDataAsset>(Manger->GetPrimaryAssetObject(LoadedId));
	if (!EnemyData) {
		return;
	}

	AActor* AEnemy = GetWorld()->SpawnActor<AActor>(EnemyData->EnemyClass, SpawnLocation, FRotator::ZeroRotator);
	if (!AEnemy) {
		return;
	}

	USActionComponent* ActionComp = Cast<USActionComponent>(AEnemy->GetComponentByClass(USActionComponent::StaticClass()));
	if (!ActionComp) {
		return;
	}

	for (TSubclassOf<USAction> Action : EnemyData->Actions) {
		ActionComp->AddAction(AEnemy, Action);
	}
}

void ASGameModeBase::OnQueryPowerItemCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Enemy EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	int32 spwanNum = FMath::Min<int32>(POWER_ITEM_SPAWN_NUM, Locations.Num());

	for (int32 i = 0; i < spwanNum; ++i) {
		int32 randomIndex = FMath::RandRange(0, POWER_ITEM_NUM - 1);
		if (Locations.IsValidIndex(i)) {
			GetWorld()->SpawnActor<AActor>(m_SpawnItem[randomIndex], Locations[i], FRotator::ZeroRotator);
		}
	}
}

void ASGameModeBase::OnActorKilled(AActor* DiedActor, AActor* KillActor)
{
	ASPlayerState* PlayerState = Cast<ASPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	if (DiedActor->ActorHasTag(FName("Enemy"))) {
		if (PlayerState) {
			PlayerState->IncreaseScore(5);
		}

	} else {
		if (PlayerState) {
			PlayerState->IncreaseScore(-5);
		}

		ASCharacter* Player = Cast<ASCharacter>(DiedActor);

		// 如果是玩家，则两秒后进行一个复活
		if (Player) {
			FTimerHandle TimeHandle_RespawnPlayer;
			FTimerDelegate TimeDelegate;

			TimeDelegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
			GetWorldTimerManager().SetTimer(TimeHandle_RespawnPlayer, TimeDelegate, 2.0f, false);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Dier: %s, Killer: %s"), *GetNameSafe(DiedActor), *GetNameSafe(KillActor));
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (Controller) {
		Controller->UnPossess();		//释放控制
		RestartPlayer(Controller);
	}
}
