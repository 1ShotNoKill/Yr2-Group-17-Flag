// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "MurderGameMode.generated.h"

UCLASS()
class MURDERMASQUERADE_API AMurderGameMode : public AGameModeBase
{
	GENERATED_BODY()
	AMurderGameMode();
public:

	UUserWidget* PlayerWidget;
	int TimeRemaining = 10;
	int GamePhase = 1;
	UPROPERTY(EditAnywhere)
	bool bIsTargetDead = false;
	int PlayerPoints = 0;

	virtual void StartPlay() override;

	void Wingame();
	void LoseGame();
	void ChangePhase(int NewPhase);
	void UpdateTimer();
	void EndGame();
	FTimerHandle Phase2Timer;
	UFUNCTION()
	void OnTargetDeath();
	UFUNCTION()
	void OnNonTargetDeath();
	UFUNCTION()
	void OnGuardDeath();
};
