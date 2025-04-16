// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayerState.h"
#include "Save/SSaveGame.h"

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveGame)
{
	if (!SaveGame) {
		return;
	}

	FPlayerSaveData SaveData;
	SaveData.Score = m_Score;
	//SaveData.PersonalRecordTime = PersonalRecordTime;
	SaveData.PlayerID = GetUniqueId().ToString();

	if (APawn* MyPawn = GetPawn()) {
		SaveData.Location = MyPawn->GetActorLocation();
		SaveData.Rotation = MyPawn->GetActorRotation();
		SaveData.bResumeAtTransform = true;
	}

	SaveGame->m_SavedPlayers.Add(SaveData);
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveGame)
{
	if (!SaveGame) {
		return;
	}

	FPlayerSaveData* FoundData = SaveGame->GetPlayerData(this);
	if (FoundData) {
		IncreaseScore(FoundData->Score);
	} else {
		UE_LOG(LogTemp, Log, TEXT("Not Found SaveData"));
	}
}

void ASPlayerState::IncreaseScore(int32 ChangeScore)
{
	m_Score += ChangeScore;
	OnScoreChanged.Broadcast(ChangeScore, m_Score, true);
}

void ASPlayerState::UnSucScoreChange(int32 ChangeScore)
{
	OnScoreChanged.Broadcast(ChangeScore, m_Score, false);
}

int32 ASPlayerState::GetPlayerScore()
{
	return m_Score;
}

