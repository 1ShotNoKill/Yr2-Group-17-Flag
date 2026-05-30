// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ItemInterationComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MURDERMASQUERADE_API UItemInterationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemInterationComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere)
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

public:
	FHitResult LineTraceViaPlayer(FName Profile, float Range);

	UFUNCTION()
	void PickupInteractable();

	UFUNCTION()
	void DropInteractable();

	UFUNCTION()
	void UseHeldItem();
};
