// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "M_MCameraManager.generated.h"

/**
 * Basic first person camera manager
 */
UCLASS()
class MURDERMASQUERADE_API AM_MCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AM_MCameraManager();
};