// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterBulletCounterUI.generated.h"

/**
 * Simple bullet counter UI widget
 */
UCLASS(Abstract)
class MURDERMASQUERADE_API UShooterBulletCounterUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooter", meta = (DisplayName = "UpdateBulletCounter"))
	void BP_UpdateBulletCounter(int32 MagazineSize, int32 BulletCount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooter", meta = (DisplayName = "Damaged"))
	void BP_Damaged(float LifePercent);
};