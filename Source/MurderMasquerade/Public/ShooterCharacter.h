// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "ShooterWeaponHolder.h"
#include "ShooterCharacter.generated.h"

class AShooterWeapon;
class UInputAction;
class UInputComponent;
class UPawnNoiseEmitterComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FBulletCountUpdatedDelegate,
	int32,
	MagazineSize,
	int32,
	Bullets
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FDamagedDelegate,
	float,
	LifePercent
);

UCLASS(Abstract)
class MURDERMASQUERADE_API AShooterCharacter
	: public APlayerCharacter,
	public IShooterWeaponHolder
{
	GENERATED_BODY()

private:

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components",
		meta = (AllowPrivateAccess = "true")
	)
	UPawnNoiseEmitterComponent* PawnNoiseEmitter;

protected:

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SwitchWeaponAction;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");

	UPROPERTY(EditAnywhere, Category = "Aim")
	float MaxAimDistance = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHP = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHP = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Team")
	uint8 TeamByte = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bHasWeapon = false;

	UPROPERTY()
	TArray<AShooterWeapon*> OwnedWeapons;

	UPROPERTY()
	TObjectPtr<AShooterWeapon> CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Respawn")
	float RespawnTime = 5.0f;

	FTimerHandle RespawnTimer;

public:

	FBulletCountUpdatedDelegate OnBulletCountUpdated;

	FDamagedDelegate OnDamaged;

public:

	AShooterCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupPlayerInputComponent(
		UInputComponent* PlayerInputComponent
	) override;

public:

	virtual float TakeDamage(
		float Damage,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintCallable, Category = "Shooter")
	void DoStartFiring();

	UFUNCTION(BlueprintCallable, Category = "Shooter")
	void DoStopFiring();

	UFUNCTION(BlueprintCallable, Category = "Shooter")
	void DoSwitchWeapon();

	void SetHasWeapon(bool bNewHasWeapon);

	bool HasWeapon() const;

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

	AShooterWeapon* FindWeaponOfType(
		TSubclassOf<AShooterWeapon> WeaponClass
	) const;

	void Die();

	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Shooter",
		meta = (DisplayName = "On Death")
	)
	void BP_OnDeath();

	void OnRespawn();
};