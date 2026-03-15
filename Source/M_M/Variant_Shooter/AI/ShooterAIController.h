// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ShooterAIController.generated.h"

class UStateTreeAIComponent;
class UAIPerceptionComponent;

DECLARE_DELEGATE_TwoParams(FShooterPerceptionUpdatedDelegate, AActor*, const FAIStimulus&);
DECLARE_DELEGATE_OneParam(FShooterPerceptionForgottenDelegate, AActor*);

/**
 * Simple AI Controller for a first person shooter enemy
 */
UCLASS(abstract)
class M_M_API AShooterAIController : public AAIController
{
    GENERATED_BODY()

    /** Runs the behavior StateTree for this NPC */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
    UStateTreeAIComponent* StateTreeAI;

    /** Detects other actors through sight, hearing and other senses */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
    UAIPerceptionComponent* AIPerception;

protected:

    /** Team tag for pawn friend or foe identification */
    UPROPERTY(EditAnywhere, Category="Shooter")
    FName TeamTag = FName("Enemy");

    /** Enemy currently being targeted */
    TObjectPtr<AActor> TargetEnemy;

public:

    /** Called when an AI perception has been updated */
    FShooterPerceptionUpdatedDelegate OnShooterPerceptionUpdated;

    /** Called when an AI perception has been forgotten */
    FShooterPerceptionForgottenDelegate OnShooterPerceptionForgotten;

public:

    /** Constructor */
    AShooterAIController();

protected:

    /** Pawn initialization */
    virtual void OnPossess(APawn* InPawn) override;

protected:

    /** Called when the possessed pawn dies */
    UFUNCTION()
    void OnPawnDeath();

public:

    /** Sets the targeted enemy */
    void SetCurrentTarget(AActor* Target);

    /** Clears the targeted enemy */
    void ClearCurrentTarget();

    /** Returns the targeted enemy */
    AActor* GetCurrentTarget() const { return TargetEnemy; }

protected:

    /** Called when perception updates */
    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    /** Called when perception forgets an actor */
    UFUNCTION()
    void OnPerceptionForgotten(AActor* Actor);
};
