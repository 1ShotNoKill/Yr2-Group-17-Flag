// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterRoom.generated.h"

UCLASS()
class MURDERMASQUERADE_API AMasterRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterRoom();

	UPROPERTY(EditAnywhere, Category = "DefaultRoom")
	bool DefaultRoom = true; //Toggle the base room to be visible in game

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UStaticMeshComponent* RoomMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
