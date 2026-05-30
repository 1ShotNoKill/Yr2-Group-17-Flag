// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterAIController.h"
#include "ShooterNPC.h"
#include "ShooterCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"

AShooterAIController::AShooterAIController()
{
    AIPerception =
        CreateDefaultSubobject<UAIPerceptionComponent>(
            TEXT("AIPerception")
        );

    AIPerception->OnTargetPerceptionUpdated.AddDynamic(
        this,
        &AShooterAIController::OnPerceptionUpdated
    );

    AIPerception->OnTargetPerceptionForgotten.AddDynamic(
        this,
        &AShooterAIController::OnPerceptionForgotten
    );
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (AShooterNPC* NPC = Cast<AShooterNPC>(InPawn))
    {
        NPC->Tags.Add(TeamTag);

        NPC->OnPawnDeath.AddDynamic(
            this,
            &AShooterAIController::OnPawnDeath
        );
    }
}

void AShooterAIController::OnPawnDeath()
{
    if (GetPathFollowingComponent())
    {
        GetPathFollowingComponent()->AbortMove(
            *this,
            FPathFollowingResultFlags::UserAbort
        );
    }

    UnPossess();

    Destroy();
}

void AShooterAIController::SetCurrentTarget(AActor* Target)
{
    TargetEnemy = Target;
}

void AShooterAIController::ClearCurrentTarget()
{
    TargetEnemy = nullptr;
}

void AShooterAIController::OnPerceptionUpdated(
    AActor* Actor,
    FAIStimulus Stimulus)
{
    AShooterCharacter* PlayerCharacter =
        Cast<AShooterCharacter>(Actor);

    if (PlayerCharacter)
    {
        if (!PlayerCharacter->HasWeapon())
        {
            ClearCurrentTarget();
            return;
        }
    }

    OnShooterPerceptionUpdated.ExecuteIfBound(
        Actor,
        Stimulus
    );
}

void AShooterAIController::OnPerceptionForgotten(AActor* Actor)
{
    OnShooterPerceptionForgotten.ExecuteIfBound(Actor);
}