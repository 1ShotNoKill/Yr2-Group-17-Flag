// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIMarker_Actor.generated.h"

UCLASS()
class MURDERMASQUERADE_API AAIMarker_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIMarker_Actor();
	bool bIsAvaliable = true;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Base;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
