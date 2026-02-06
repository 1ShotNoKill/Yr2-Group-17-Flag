// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInteractionInterface.h"

#include "InteractableItem_Master.generated.h"

UCLASS()
class MURDERMASQUERADE_API AInteractableItem_Master : public AActor, public IItemInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableItem_Master();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Custom Variables
public:

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
private:


	//Custom Functions
public:
protected:
	void PickUpItem_Implementation(APlayerCharacter* Interactor);
	virtual void DropItem_Implementation(APlayerCharacter* Interactor);
	virtual void UseItem_Implementation(APlayerCharacter* Interactor);
private:


};
