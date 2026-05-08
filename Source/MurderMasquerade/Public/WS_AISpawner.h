// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIPositionMarker_Master.h"
#include "MurderGameMode.h"
#include "WS_AISpawner.generated.h"

class AAIPositionMarker_Master;
class AAIMarker_Actor;

UENUM(BlueprintType)
enum class EMask : uint8
{
	None UMETA(DisplayName = "None"),
	Bird UMETA(DisplayName = "Bird Mask"),
	Bear UMETA(DisplayName = "Bear Mask"),
	Rabbit UMETA(DisplayName = "Rabbit Mask")
};


UCLASS()
class MURDERMASQUERADE_API UWS_AISpawner : public UWorldSubsystem
{
	GENERATED_BODY()
	UWS_AISpawner();
public:
	AAIMarker_Actor* GetRandomMarker();
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> AllMarkers;
	AActor* GetTarget();
	int MaxGuests = 10;
protected:
	EMask TargetMask;
	AActor* Target;
private:
	virtual void OnWorldBeginPlay(UWorld& World) override;
	void FindAllMarkerActors();
	void SpawnGuests();
	EMask GetRandomMask();
	void UpdateGamemodeMask();


	TMap<EMask, FString> MaskMap;
	
	FActorSpawnParameters SpawnParams;
};
