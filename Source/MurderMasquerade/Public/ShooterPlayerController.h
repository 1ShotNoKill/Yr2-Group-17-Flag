// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

class UInputMappingContext;
class AShooterCharacter;
class UShooterBulletCounterUI;
class UUserWidget;

UCLASS(Abstract)
class MURDERMASQUERADE_API AShooterPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:

    UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
    TArray<UInputMappingContext*> DefaultMappingContexts;

    UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
    TArray<UInputMappingContext*> MobileExcludedMappingContexts;

    UPROPERTY(EditAnywhere, Category = "Input|Touch Controls")
    TSubclassOf<UUserWidget> MobileControlsWidgetClass;

    TObjectPtr<UUserWidget> MobileControlsWidget;

    UPROPERTY(EditAnywhere, Category = "Shooter|Respawn")
    TSubclassOf<AShooterCharacter> CharacterClass;

    UPROPERTY(EditAnywhere, Category = "Shooter|UI")
    TSubclassOf<UShooterBulletCounterUI> BulletCounterUIClass;

    UPROPERTY(EditAnywhere, Category = "Shooter|Player")
    FName PlayerPawnTag = FName("Player");

    TObjectPtr<UShooterBulletCounterUI> BulletCounterUI;

protected:

    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION()
    void OnPawnDestroyed(AActor* DestroyedActor);

    UFUNCTION()
    void OnBulletCountUpdated(int32 MagazineSize, int32 Bullets);

    UFUNCTION()
    void OnPawnDamaged(float LifePercent);
};