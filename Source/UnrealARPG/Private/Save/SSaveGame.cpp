// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/SSaveGame.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"

FPlayerSaveData* USSaveGame::GetPlayerData(APlayerState* PlayerState)
{
	if (!PlayerState) {
		return nullptr;
	}

	//当PIE时不会给出唯一的ID，所以我们在编辑器中测试时跳过这一步。
	if (PlayerState->GetWorld()->IsPlayInEditor()) {
		if (m_SavedPlayers.IsValidIndex(0)) {
			return &m_SavedPlayers[0];
		}

		return nullptr;
	} else {
		FString PlayerID = PlayerState->GetUniqueId().ToString();
		for (const FPlayerSaveData& Data : m_SavedPlayers) {
			if (Data.PlayerID == PlayerID) {
				return const_cast<FPlayerSaveData*>(&Data);
			}
		}
	}

	return nullptr;
}
