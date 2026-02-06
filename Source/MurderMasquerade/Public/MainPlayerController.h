// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/Userwidget.h"


#include "MainPlayerController.generated.h"


UCLASS()
class MURDERMASQUERADE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPlayerController();
	virtual void BeginPlay() override;

	UPROPERTY()
	TSubclassOf<UUserWidget> PlayerHudClass;

	UPROPERTY()
	UUserWidget* PlayerHudReference;
	
};
