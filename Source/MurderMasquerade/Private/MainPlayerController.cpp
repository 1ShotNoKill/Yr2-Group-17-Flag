// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

AMainPlayerController::AMainPlayerController()
{
	ConstructorHelpers::FClassFinder<UUserWidget> LoadedWidgetClass (TEXT("/Game/Player/Hud/PlayerHud_Widget.PlayerHud_Widget_C"));
	if (LoadedWidgetClass.Succeeded()) PlayerHudClass = LoadedWidgetClass.Class;
}

void AMainPlayerController::BeginPlay()
{
	if (IsLocalController() && PlayerHudClass)
	{
			PlayerHudReference = CreateWidget<UUserWidget>(this, PlayerHudClass);
			PlayerHudReference->AddToViewport();
	}
}
