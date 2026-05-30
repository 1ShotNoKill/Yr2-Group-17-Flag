#include "MurderGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

AMurderGameMode::AMurderGameMode()
{
	// Sets PlayerCharacter as default Pawn
	DefaultPawnClass = APlayerCharacter::StaticClass();

	// Default Pawn uses MainPlayerController as controller
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

bool AMurderGameMode::IsEscapeActive() const
{
	return bEscapeActive;
}

void AMurderGameMode::PlayerEscaped()
{
	if (!bEscapeActive || bPlayerEscaped)
	{
		return;
	}

	bPlayerEscaped = true;

	GetWorldTimerManager().ClearTimer(EscapeTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("PLAYER ESCAPED"));

	Wingame();
}

void AMurderGameMode::StartPlay()
{
	Super::StartPlay();

	if (Phase2Timer.IsValid())
	{
		Phase2Timer.Invalidate();
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(
		UGameplayStatics::GetActorOfClass(
			GetWorld(),
			APlayerCharacter::StaticClass()
		)
	);

	if (Player)
	{
		AMainPlayerController* PlayerController =
			Cast<AMainPlayerController>(Player->GetController());

		PlayerWidget = PlayerController->PlayerHudReference;
	}
}

void AMurderGameMode::Wingame()
{
	if (PlayerWidget)
	{
		if (FProperty* WinText =
			PlayerWidget->GetClass()->FindPropertyByName("Txt_Success"))
		{
			UFunction* WinFunct =
				PlayerWidget->FindFunction(TEXT("SetWin"));

			if (WinFunct)
			{
				struct FUpdateWin
				{
					bool Win;
				};

				FUpdateWin WinParams;
				WinParams.Win = true;

				PlayerWidget->ProcessEvent(WinFunct, &WinParams);
			}
		}
	}
}

void AMurderGameMode::LoseGame()
{
	if (PlayerWidget)
	{
		if (FProperty* LoseText =
			PlayerWidget->GetClass()->FindPropertyByName("Txt_Failed"))
		{
			UFunction* LoseFunct =
				PlayerWidget->FindFunction(TEXT("SetLoss"));

			if (LoseFunct)
			{
				struct FUpdateTextParams
				{
					bool Loss;
				};

				FUpdateTextParams LoseParams;
				LoseParams.Loss = true;

				PlayerWidget->ProcessEvent(LoseFunct, &LoseParams);
			}
		}
	}
}

void AMurderGameMode::ChangePhase(int NewPhase)
{
	GamePhase = NewPhase;

	// PHASE 2 = ESCAPE PHASE
	if (NewPhase == 2)
	{
		StartEscapeTimer();
	}
}

void AMurderGameMode::StartEscapeTimer()
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
		&AMurderGameMode::EscapeTimerTick,
		1.0f,
		true
	);
}

void AMurderGameMode::EscapeTimerTick()
{
	EscapeTimeRemaining--;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Escape Time Remaining: %.0f"),
		EscapeTimeRemaining
	);

	// Update widget timer if your widget supports it
	if (PlayerWidget)
	{
		UFunction* Func =
			PlayerWidget->FindFunction(TEXT("UpdateText"));

		if (Func)
		{
			struct FUpdateTextParams
			{
				int32 Int;
			};

			FUpdateTextParams TxtParams;
			TxtParams.Int = EscapeTimeRemaining;

			PlayerWidget->ProcessEvent(Func, &TxtParams);
		}
	}

	if (EscapeTimeRemaining <= 0)
	{
		OnEscapeTimerExpired();
	}
}

void AMurderGameMode::OnEscapeTimerExpired()
{
	GetWorldTimerManager().ClearTimer(EscapeTimerHandle);

	if (!bPlayerEscaped)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER FAILED TO ESCAPE"));

		LoseGame();
	}
}

void AMurderGameMode::UpdateTimer()
{
	// Old timer logic no longer needed
}

void AMurderGameMode::EndGame()
{
	FString FullMapName =
		UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	UGameplayStatics::OpenLevel(GetWorld(), *FullMapName);
}

void AMurderGameMode::UpdateMaskDesc(FString Mask)
{
	if (PlayerWidget && !Mask.IsEmpty())
	{
		if (FProperty* Maskdesc =
			PlayerWidget->GetClass()->FindPropertyByName("Txt_MaskDescription"))
		{
			UFunction* MaskdescFunc =
				PlayerWidget->FindFunction(TEXT("SetMaskText"));

			if (MaskdescFunc)
			{
				struct FUpdateMaskdesc
				{
					FText Text;
				};

				FUpdateMaskdesc UpdateParams;

				UpdateParams.Text = FText::FromString(Mask);

				PlayerWidget->ProcessEvent(
					MaskdescFunc,
					&UpdateParams
				);
			}
		}
	}
}

void AMurderGameMode::OnTargetDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Target Has Died"));

	UE_LOG(LogTemp, Warning, TEXT("1000 points"));

	bIsTargetDead = true;

	PlayerPoints += 1000;

	// Start escape phase
	ChangePhase(2);
}

void AMurderGameMode::OnNonTargetDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("-500 points"));

	PlayerPoints -= 500;
}

void AMurderGameMode::OnGuardDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("+200 points"));

	PlayerPoints += 200;
}
