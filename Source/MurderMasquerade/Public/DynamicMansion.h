// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicMansion.generated.h"


UCLASS()
class MURDERMASQUERADE_API ADynamicMansion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicMansion();


	UPROPERTY(EditAnywhere, Category = "Hallway")
	int HallLength;
	UPROPERTY(EditAnywhere, Category = "Hallway")
	int HallspacerStart;
	UPROPERTY(EditAnywhere, Category = "Hallway")
	int HallspacerEnd;

	//Prefabs to randomly pick from
	UPROPERTY(EditAnywhere, Category = "Prefabs")
	TArray<TSoftClassPtr<class AMasterHallwaySegment>> HallwayPrefabs;
	UPROPERTY(EditAnywhere, Category = "Prefabs")
	TArray<TSoftClassPtr<class AMasterSpacerSegment>> SpacerPrefabs;
	UPROPERTY(EditAnywhere, Category = "Prefabs")
	TArray<TSoftClassPtr<class AMasterRoom>> RoomPrefabs;


protected:
	UPROPERTY(EditAnywhere, Category = "Hallway")
	float HallwayOffset = 300;
	UPROPERTY(EditAnywhere, Category = "Hallway")
	float SpacerOffset = 300;
private:
	//Spawned Segments/Rooms
	UPROPERTY()	
	UStaticMeshComponent* FrontDoor;
	UPROPERTY()	
	TArray<UChildActorComponent*> HallwaySegments;
	UPROPERTY()
	TArray<UChildActorComponent*> Rooms;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	virtual void OnConstruction(const FTransform& Transform) override;
	FVector GetLastSegmentPosition(float Offset);
	void GetDefaults();
	TArray<FAssetData> FindAssets(FName Directory);
};
