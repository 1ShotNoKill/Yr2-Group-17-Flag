// Fill out your copyright notice in the Description page of Project Settings.


#include "MurderGameMode.h"

#include <Kismet/GameplayStatics.h>

AMurderGameMode::AMurderGameMode()
{
	//sets PlayerCharacter as default Pawn
	DefaultPawnClass = APlayerCharacter::StaticClass();
	//Default Pawn uses MainPlayerController as controller
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AMurderGameMode::StartPlay()
{
	Super::StartPlay();

	if(Phase2Timer.IsValid())Phase2Timer.Invalidate();
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
	if (Player)
	{
		AMainPlayerController* PlayerController = Cast<AMainPlayerController>(Player->GetController());
		PlayerWidget = PlayerController->PlayerHudReference;
	}

}

void AMurderGameMode::Wingame()
{
	if (PlayerWidget)
	{
		if (FProperty* WinText = PlayerWidget->GetClass()->FindPropertyByName("Txt_Success"))
		{
			UFunction* WinFunct = PlayerWidget->FindFunction(TEXT("SetWin"));
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
		if (FProperty* LoseText = PlayerWidget->GetClass()->FindPropertyByName("Txt_Failed"))
		{
			UFunction* LoseFunct = PlayerWidget->FindFunction(TEXT("SetLoss"));
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
	if (NewPhase == 2)
	{
		GetWorld()->GetTimerManager().SetTimer(Phase2Timer, this, &AMurderGameMode::UpdateTimer, 1.f, true);
	}
}

void AMurderGameMode::EndGame() 
{
	FString FullMapName = GetWorld()->GetMapName();
	FString ShortMapName = FPackageName::GetShortName(FullMapName);

	UGameplayStatics::OpenLevel(GetWorld(), *ShortMapName);
}

void AMurderGameMode::UpdateTimer()
{
	if (bIsTargetDead == true || TimeRemaining < 5) Wingame();
	else if (bIsTargetDead == false || TimeRemaining < 5) LoseGame();
	if (TimeRemaining == -10)
	{
		EndGame();
		Phase2Timer.Invalidate();
	}
	TimeRemaining = TimeRemaining -1;
	if (TimeRemaining >= 0)
	{


		UFunction* Func = PlayerWidget->FindFunction(TEXT("UpdateText"));
		if (Func)
		{
			UE_LOG(LogTemp, Warning, TEXT("Func is valid"));
			struct FUpdateTextParams
			{
				int32 Int;
			};

			FUpdateTextParams TxtParams;
			TxtParams.Int = TimeRemaining;
			PlayerWidget->ProcessEvent(Func, &TxtParams);
		}
	}
}

void AMurderGameMode::OnTargetDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Target Has Died"));
	UE_LOG(LogTemp, Warning, TEXT("1000 points"));

	bIsTargetDead = true;
	PlayerPoints += 1000;

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
