// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIMarker_Actor.h"
#include "AIPositionMarker_Master.generated.h"

UCLASS()
class MURDERMASQUERADE_API AAIPositionMarker_Master : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIPositionMarker_Master();
	void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UChildActorComponent*> MarkersParent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* FocusTarget;

	TArray<AActor*> GetAllMarkerActors();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
