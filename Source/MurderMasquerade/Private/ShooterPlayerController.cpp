// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ShooterCharacter.h"
#include "ShooterBulletCounterUI.h"

#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalPlayerController())
    {
        MobileControlsWidget =
            CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

        if (MobileControlsWidget)
        {
            MobileControlsWidget->AddToPlayerScreen();
        }

        BulletCounterUI =
            CreateWidget<UShooterBulletCounterUI>(
                this,
                BulletCounterUIClass
            );

        if (BulletCounterUI)
        {
            BulletCounterUI->AddToPlayerScreen();
        }
    }
}

void AShooterPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (IsLocalPlayerController())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                GetLocalPlayer()))
        {
            for (UInputMappingContext* Context : DefaultMappingContexts)
            {
                if (Context)
                {
                    Subsystem->AddMappingContext(Context, 0);
                }
            }

            for (UInputMappingContext* Context :
                MobileExcludedMappingContexts)
            {
                if (Context)
                {
                    Subsystem->AddMappingContext(Context, 0);
                }
            }
        }
    }
}

void AShooterPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn)
    {
        return;
    }

    InPawn->OnDestroyed.AddDynamic(
        this,
        &AShooterPlayerController::OnPawnDestroyed
    );

    if (AShooterCharacter* ShooterCharacter =
        Cast<AShooterCharacter>(InPawn))
    {
        ShooterCharacter->Tags.Add(PlayerPawnTag);

        ShooterCharacter->OnBulletCountUpdated.AddDynamic(
            this,
            &AShooterPlayerController::OnBulletCountUpdated
        );

        ShooterCharacter->OnDamaged.AddDynamic(
            this,
            &AShooterPlayerController::OnPawnDamaged
        );

        ShooterCharacter->OnDamaged.Broadcast(1.0f);
    }
}

void AShooterPlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{
    if (BulletCounterUI)
    {
        BulletCounterUI->BP_UpdateBulletCounter(0, 0);
    }

    TArray<AActor*> PlayerStarts;

    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        APlayerStart::StaticClass(),
        PlayerStarts
    );

    if (PlayerStarts.Num() > 0)
    {
        AActor* RandomStart =
            PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)];

        const FTransform SpawnTransform =
            RandomStart->GetActorTransform();

        if (AShooterCharacter* RespawnedCharacter =
            GetWorld()->SpawnActor<AShooterCharacter>(
                CharacterClass,
                SpawnTransform))
        {
            Possess(RespawnedCharacter);
        }
    }
}

void AShooterPlayerController::OnBulletCountUpdated(
    int32 MagazineSize,
    int32 Bullets)
{
    if (BulletCounterUI)
    {
        BulletCounterUI->BP_UpdateBulletCounter(
            MagazineSize,
            Bullets
        );
    }
}

void AShooterPlayerController::OnPawnDamaged(float LifePercent)
{
    if (BulletCounterUI)
    {
        BulletCounterUI->BP_Damaged(LifePercent);
    }
}