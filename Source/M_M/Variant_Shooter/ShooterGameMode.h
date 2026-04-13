// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

UCLASS()
class M_M_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AShooterGameMode();

	virtual void BeginPlay() override;

	// Called when the enemy dies
	void StartEscapeTimer();

	// Called when the player leaves the exit zone
	void PlayerEscaped();

	// Check if escape phase is active
	bool IsEscapeActive() const;

protected:

	void EscapeTimerTick();

	void OnEscapeTimerExpired();

	void OnPlayerWin();

	void RestartLevel();

protected:

	// Total escape time
	UPROPERTY(EditAnywhere, Category = "Escape")
	float EscapeTime = 60.0f;

	// Delay before win message
	UPROPERTY(EditAnywhere, Category = "Escape")
	float WinDelay = 3.0f;

	float EscapeTimeRemaining;

	bool bEscapeActive = false;

	bool bPlayerEscaped = false;

	FTimerHandle EscapeTimerHandle;

	FTimerHandle WinTimerHandle;
};
