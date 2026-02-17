// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <NavigationSystem.h>
#include "CrowdAIController.h"
#include "WS_AISpawner.h"

#include "Master_Ai.generated.h"

class AAIMarker_Actor;

UCLASS()
class MURDERMASQUERADE_API AMaster_Ai : public ACharacter
{
	GENERATED_BODY()

//Default
public:
	AMaster_Ai(); 	// Sets default values for this character's properties
protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned
public:
	virtual void Tick(float DeltaTime) override; 	// Called every frame
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
//Variables
public:
	UWS_AISpawner* Spawner;
protected:
	AAIMarker_Actor* CurrentMarker;

	AAIController* AIController;
	ACrowdAIController* AIC;
	UNavigationSystemV1* NavSystem;
private:

//Functions
public:
protected:
	void MoveToRandomLocation(float Range);
	void MoveToRandomMarker();
	void test();
	UFUNCTION()
	void RotateCharacter(FAIRequestID ID, EPathFollowingResult::Type Result);
private:
};
