// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <PlayerCharacter.h>
#include "ItemInteractionInterface.generated.h"



UINTERFACE(MinimalAPI)
class UItemInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class MURDERMASQUERADE_API IItemInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PickUpItem(APlayerCharacter* Interactor);
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void DropItem(APlayerCharacter* Interactor);
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void UseItem(APlayerCharacter* Interactor);

};
