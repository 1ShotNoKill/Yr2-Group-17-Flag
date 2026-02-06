// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ItemInterationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MURDERMASQUERADE_API UItemInterationComponent : public UActorComponent
{
	GENERATED_BODY()

	/*Defaults*/
public:
	// Sets default values for this component's properties
	UItemInterationComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//Custom Variables
public:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* AttachPoint;
	bool bIsHoldingItem;
protected:
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float PickupRange = 500.f;
	
private:
	UPROPERTY(VisibleAnywhere)
	class APlayerCharacter* Owner;
	UPROPERTY(VisibleAnywhere)
	AActor* HeldActor;
	//Used to attach interactables to and is attached to player somewhere
	
//Custom Functions
public:
	FHitResult LineTraceViaPlayer(FName Profile,float Range);

	UFUNCTION()
	void PickupInteractable();
	UFUNCTION()
	void DropInteractable();
	UFUNCTION()
	void UseHeldItem();
//protected:
//
//private:
	
};
