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

	// Escape system
	UFUNCTION(BlueprintCallable)
	bool IsEscapeActive() const;

	UFUNCTION(BlueprintCallable)
	void PlayerEscaped();

	void StartEscapeTimer();

	// Existing variables
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

	void UpdateMaskDesc(FString Mask);

	UFUNCTION()
	void OnTargetDeath();

	UFUNCTION()
	void OnNonTargetDeath();

	UFUNCTION()
	void OnGuardDeath();

protected:

	// Escape Timer Functions
	void EscapeTimerTick();

	void OnEscapeTimerExpired();

	// Existing timer
	FTimerHandle Phase2Timer;

	// Escape system variables
	UPROPERTY(EditAnywhere, Category = "Escape")
	float EscapeTime = 60.0f;

	float EscapeTimeRemaining;

	bool bEscapeActive = false;

	bool bPlayerEscaped = false;

	FTimerHandle EscapeTimerHandle;
};
