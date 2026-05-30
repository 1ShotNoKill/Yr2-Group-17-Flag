// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ShooterAIController.generated.h"

class UAIPerceptionComponent;

DECLARE_DELEGATE_TwoParams(FShooterPerceptionUpdatedDelegate, AActor*, const FAIStimulus&);
DECLARE_DELEGATE_OneParam(FShooterPerceptionForgottenDelegate, AActor*);

UCLASS(Blueprintable)
class MURDERMASQUERADE_API AShooterAIController : public AAIController
{
    GENERATED_BODY()

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAIPerceptionComponent* AIPerception;

    UPROPERTY(EditAnywhere, Category = "Shooter")
    FName TeamTag = FName("Enemy");

    UPROPERTY()
    TObjectPtr<AActor> TargetEnemy;

public:

    FShooterPerceptionUpdatedDelegate OnShooterPerceptionUpdated;

    FShooterPerceptionForgottenDelegate OnShooterPerceptionForgotten;

public:

    AShooterAIController();

protected:

    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION()
    void OnPawnDeath();

public:

    void SetCurrentTarget(AActor* Target);

    void ClearCurrentTarget();

    AActor* GetCurrentTarget() const
    {
        return TargetEnemy;
    }

protected:

    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    UFUNCTION()
    void OnPerceptionForgotten(AActor* Actor);
};