// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "ShooterWeaponHolder.h"
#include "ShooterNPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPawnDeathDelegate);

class AShooterWeapon;

UCLASS(Abstract)
class MURDERMASQUERADE_API AShooterNPC
	: public APlayerCharacter,
	public IShooterWeaponHolder
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float CurrentHP = 100.0f;

protected:

	UPROPERTY(EditAnywhere, Category = "Damage")
	FName RagdollCollisionProfile = FName("Ragdoll");

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DeferredDestructionTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Team")
	uint8 TeamByte = 1;

	UPROPERTY()
	TObjectPtr<AShooterWeapon> Weapon;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AShooterWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");

	UPROPERTY(EditAnywhere, Category = "Aim")
	float AimRange = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Aim")
	float AimVarianceHalfAngle = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Aim")
	float MinAimOffsetZ = -35.0f;

	UPROPERTY(EditAnywhere, Category = "Aim")
	float MaxAimOffsetZ = -60.0f;

	UPROPERTY()
	TObjectPtr<AActor> CurrentAimTarget;

	bool bIsShooting = false;

	bool bIsDead = false;

	FTimerHandle DeathTimer;

public:

	UPROPERTY(BlueprintAssignable)
	FPawnDeathDelegate OnPawnDeath;

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(
		const EEndPlayReason::Type EndPlayReason
	) override;

public:

	virtual float TakeDamage(
		float Damage,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

public:

	virtual void AttachWeaponMeshes(
		AShooterWeapon* Weapon
	) override;

	virtual void PlayFiringMontage(
		UAnimMontage* Montage
	) override;

	virtual void AddWeaponRecoil(
		float Recoil
	) override;

	virtual void UpdateWeaponHUD(
		int32 CurrentAmmo,
		int32 MagazineSize
	) override;

	virtual FVector GetWeaponTargetLocation() override;

	virtual void AddWeaponClass(
		const TSubclassOf<AShooterWeapon>& WeaponClass
	) override;

	virtual void OnWeaponActivated(
		AShooterWeapon* Weapon
	) override;

	virtual void OnWeaponDeactivated(
		AShooterWeapon* Weapon
	) override;

	virtual void OnSemiWeaponRefire() override;

protected:

	void Die();

	void DeferredDestruction();

public:

	void StartShooting(AActor* ActorToShoot);

	void StopShooting();
};