// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

UCLASS()
class MURDERMASQUERADE_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AShooterGameMode();

	virtual void BeginPlay() override;

	void StartEscapeTimer();

	void PlayerEscaped();

	bool IsEscapeActive() const;

protected:

	void EscapeTimerTick();

	void OnEscapeTimerExpired();

	void OnPlayerWin();

	void RestartLevel();

protected:

	UPROPERTY(EditAnywhere, Category = "Escape")
	float EscapeTime = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Escape")
	float WinDelay = 3.0f;

	float EscapeTimeRemaining;

	bool bEscapeActive = false;

	bool bPlayerEscaped = false;

	FTimerHandle EscapeTimerHandle;

	FTimerHandle WinTimerHandle;
};