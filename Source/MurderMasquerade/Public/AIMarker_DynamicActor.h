// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIMarker_Actor.h"
#include "AIPositionMarker_Master.h"
#include <GameFramework/SpringArmComponent.h>

#include "AIMarker_DynamicActor.generated.h"


/**
 * 
 */
UCLASS()
class MURDERMASQUERADE_API AAIMarker_DynamicActor : public AAIPositionMarker_Master
{
	GENERATED_BODY()
public:
	AAIMarker_DynamicActor();

	void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DisplayObject")
	UStaticMesh* ObjectMesh;

	UPROPERTY(VisibleAnywhere);
	UStaticMeshComponent* FocusActor;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DisplayObject")
	float SpringArmDistance;
};
