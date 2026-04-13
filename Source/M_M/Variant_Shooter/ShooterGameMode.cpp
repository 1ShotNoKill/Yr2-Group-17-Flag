// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

AShooterGameMode::AShooterGameMode()
{
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}

void AShooterGameMode::StartEscapeTimer()
{
	if (bEscapeActive)
	{
		return;
	}

	bEscapeActive = true;
	bPlayerEscaped = false;

	EscapeTimeRemaining = EscapeTime;

	GetWorldTimerManager().SetTimer(
		EscapeTimerHandle,
		this,
		&AShooterGameMode::EscapeTimerTick,
		1.0f,
		true
	);
}

void AShooterGameMode::EscapeTimerTick()
{
	EscapeTimeRemaining--;

	// Show timer on screen
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			1.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Escape Time: %.0f"), EscapeTimeRemaining)
		);
	}

	if (EscapeTimeRemaining <= 0 && !bPlayerEscaped)
	{
		OnEscapeTimerExpired();
	}
}

void AShooterGameMode::OnEscapeTimerExpired()
{
	GetWorldTimerManager().ClearTimer(EscapeTimerHandle);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Red,
			TEXT("YOU FAILED")
		);
	}

	RestartLevel();
}

void AShooterGameMode::PlayerEscaped()
{
	if (!bEscapeActive || bPlayerEscaped)
	{
		return;
	}

	bPlayerEscaped = true;

	GetWorldTimerManager().SetTimer(
		WinTimerHandle,
		this,
		&AShooterGameMode::OnPlayerWin,
		WinDelay,
		false
	);
}

void AShooterGameMode::OnPlayerWin()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Green,
			TEXT("YOU WIN")
		);
	}

	RestartLevel();
}

void AShooterGameMode::RestartLevel()
{
	UGameplayStatics::OpenLevel(
		GetWorld(),
		FName(*GetWorld()->GetName())
	);
}

bool AShooterGameMode::IsEscapeActive() const
{
	return bEscapeActive;
}
