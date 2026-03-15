// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "M_MGameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class AM_MGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AM_MGameMode();
};



