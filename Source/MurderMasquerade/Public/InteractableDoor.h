// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
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
	void OpenDoor();
	virtual void Tick(float DeltaTime) override;
	

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Center;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RightDoor;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeftDoor;
	UPROPERTY(EditAnywhere)
	bool BDoorOpen = false;
	float DoorSpeed = 1;
	float TargetDegree = 100;
	FRotator StartDegree;
	FTimerHandle OpenHandle;



};
