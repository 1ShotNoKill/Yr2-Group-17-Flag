// Fill out your copyright notice in the Description page of Project Settings.


#include "MurderGameMode.h"
#include "PlayerCharacter.h"
#include "MainPlayerController.h"

AMurderGameMode::AMurderGameMode()
{
	//sets PlayerCharacter as default Pawn
	DefaultPawnClass = APlayerCharacter::StaticClass();
	//Default Pawn uses MainPlayerController as controller
	PlayerControllerClass = AMainPlayerController::StaticClass();
}
