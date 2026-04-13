// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Shooter/AI/ShooterAIController.h"
#include "ShooterNPC.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/Navigation/PathFollowingAgentInterface.h"

AShooterAIController::AShooterAIController()
{
    // Create the StateTree component
    StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAI"));

    // Create the AI perception component (configured in Blueprint)
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

    // Subscribe to perception delegates
    AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AShooterAIController::OnPerceptionUpdated);
    AIPerception->OnTargetPerceptionForgotten.AddDynamic(this, &AShooterAIController::OnPerceptionForgotten);
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Ensure we are possessing an NPC
    if (AShooterNPC* NPC = Cast<AShooterNPC>(InPawn))
    {
        // Add the team tag to the pawn
        NPC->Tags.Add(TeamTag);

        // Subscribe to the pawn death delegate
        NPC->OnPawnDeath.AddDynamic(this, &AShooterAIController::OnPawnDeath);
    }
}

void AShooterAIController::OnPawnDeath()
{
    // Stop movement
    GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::UserAbort);

    // Stop StateTree logic
    StateTreeAI->StopLogic(FString(""));

    // Unpossess the pawn
    UnPossess();

    // Destroy this controller
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

void AShooterAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // Pass the data to the StateTree delegate
    OnShooterPerceptionUpdated.ExecuteIfBound(Actor, Stimulus);
}

void AShooterAIController::OnPerceptionForgotten(AActor* Actor)
{
    // Pass the data to the StateTree delegate
    OnShooterPerceptionForgotten.ExecuteIfBound(Actor);
}
