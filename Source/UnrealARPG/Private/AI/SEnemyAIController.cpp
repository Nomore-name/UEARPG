// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SEnemyAIController.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>

void ASEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(m_EnemyBehaviorTree);

	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//
	//if (PlayerPawn) {
	//	if (GetBlackboardComponent()) {
	//		GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	//	}
	//}
}
