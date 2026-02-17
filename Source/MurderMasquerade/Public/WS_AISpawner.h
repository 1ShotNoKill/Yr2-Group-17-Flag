// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIPositionMarker_Master.h"
#include "WS_AISpawner.generated.h"

class AAIPositionMarker_Master;
class AAIMarker_Actor;

UENUM(BlueprintType)
enum class EMask : uint8
{
	None UMETA(DisplayName = "None"),
	Bird UMETA(DisplayName = "Bird"),
	Bear UMETA(DisplayName = "Bear"),
	Rabbit UMETA(DisplayName = "Rabbit")
};


UCLASS()
class MURDERMASQUERADE_API UWS_AISpawner : public UWorldSubsystem
{
	GENERATED_BODY()
	UWS_AISpawner();
public:
	AAIMarker_Actor* GetRandomMarker();
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> MarkerActors;

private:
	virtual void OnWorldBeginPlay(UWorld& World) override;
	void FindAllMarkerActors();
	void SpawnGuests(int MaxGuests);
	EMask GetRandomMask();


	TMap<EMask, FString> MaskMap;
	EMask TargetMask = EMask::None;
	FActorSpawnParameters SpawnParams;
};
