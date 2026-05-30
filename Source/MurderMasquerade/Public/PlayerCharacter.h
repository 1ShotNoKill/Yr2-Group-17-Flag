// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

#include "ItemInterationComponent.h"

#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDropItemDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPrimaryFireDelegate);

UCLASS()
class MURDERMASQUERADE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Damage System
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

public:

	FInteractDelegate InteractDelegate;
	FDropItemDelegate DropItemDelegate;
	FPrimaryFireDelegate PrimaryFireDelegate;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	float CurrentHealth;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	UItemInterationComponent* ItemComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TMap<FName, const UInputAction*> InputActions;

public:

	FVector GetCameraLocation();
	FVector GetCameraForwardVector();

private:

	void CreateCamera();

	void Look(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);

	void Interact();

	void DropItem();

	void PrimaryFire();

	void PauseGame();

	void Die();
};
