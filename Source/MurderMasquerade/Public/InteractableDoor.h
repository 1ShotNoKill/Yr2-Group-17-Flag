// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableItem_Master.h"
#include "InteractableDoor.generated.h"

/**
 * 
 */
UCLASS()
class MURDERMASQUERADE_API AInteractableDoor : public AInteractableItem_Master
{
	GENERATED_BODY()
	
public:
	AInteractableDoor();
	virtual void BeginPlay() override;
	void PickUpItem_Implementation(APlayerCharacter* Interactor) override;
	void ToggleDoor();
	void OpenDoor();
	virtual void Tick(float DeltaTime) override;
	

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Center;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RightDoor;
	bool BDoorOpen = false;
	float DoorSpeed = 2;
	float TargetDegree = -100;
	float StartDegree;
	FTimerHandle OpenHandle;



};
