// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterCharacter.h"
#include "ShooterWeapon.h"

#include "EnhancedInputComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AShooterCharacter::AShooterCharacter()
{
	PawnNoiseEmitter =
		CreateDefaultSubobject<UPawnNoiseEmitterComponent>(
			TEXT("PawnNoiseEmitter")
		);

	GetCharacterMovement()->RotationRate =
		FRotator(0.f, 600.f, 0.f);
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

	OnDamaged.Broadcast(1.0f);
}

void AShooterCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void AShooterCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (FireAction)
		{
			EnhancedInput->BindAction(
				FireAction,
				ETriggerEvent::Started,
				this,
				&AShooterCharacter::DoStartFiring
			);

			EnhancedInput->BindAction(
				FireAction,
				ETriggerEvent::Completed,
				this,
				&AShooterCharacter::DoStopFiring
			);
		}

		if (SwitchWeaponAction)
		{
			EnhancedInput->BindAction(
				SwitchWeaponAction,
				ETriggerEvent::Triggered,
				this,
				&AShooterCharacter::DoSwitchWeapon
			);
		}
	}
}

float AShooterCharacter::TakeDamage(
	float Damage,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	if (CurrentHP <= 0.0f)
	{
		return 0.0f;
	}

	CurrentHP -= Damage;

	OnDamaged.Broadcast(
		FMath::Clamp(CurrentHP / MaxHP, 0.0f, 1.0f)
	);

	if (CurrentHP <= 0.0f)
	{
		Die();
	}

	return Damage;
}

void AShooterCharacter::DoStartFiring()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFiring();
	}
}

void AShooterCharacter::DoStopFiring()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFiring();
	}
}

void AShooterCharacter::DoSwitchWeapon()
{
	if (OwnedWeapons.Num() <= 1 || !CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->DeactivateWeapon();

	int32 WeaponIndex = OwnedWeapons.Find(CurrentWeapon);

	if (WeaponIndex == INDEX_NONE)
	{
		return;
	}

	WeaponIndex = (WeaponIndex + 1) % OwnedWeapons.Num();

	CurrentWeapon = OwnedWeapons[WeaponIndex];

	if (CurrentWeapon)
	{
		CurrentWeapon->ActivateWeapon();
	}
}

void AShooterCharacter::AttachWeaponMeshes(
	AShooterWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	const FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		false
	);

	Weapon->AttachToActor(this, AttachRules);

	Weapon->GetThirdPersonMesh()->AttachToComponent(
		GetMesh(),
		AttachRules,
		ThirdPersonWeaponSocket
	);
}

void AShooterCharacter::PlayFiringMontage(
	UAnimMontage* Montage)
{
}

void AShooterCharacter::AddWeaponRecoil(float Recoil)
{
	AddControllerPitchInput(Recoil);
}

void AShooterCharacter::UpdateWeaponHUD(
	int32 CurrentAmmo,
	int32 MagazineSize)
{
	OnBulletCountUpdated.Broadcast(
		MagazineSize,
		CurrentAmmo
	);
}

FVector AShooterCharacter::GetWeaponTargetLocation()
{
	APlayerController* PC =
		Cast<APlayerController>(GetController());

	if (!PC)
	{
		return GetActorForwardVector() * MaxAimDistance;
	}

	FVector CameraLocation;
	FRotator CameraRotation;

	PC->GetPlayerViewPoint(
		CameraLocation,
		CameraRotation
	);

	const FVector End =
		CameraLocation +
		(CameraRotation.Vector() * MaxAimDistance);

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(
		Hit,
		CameraLocation,
		End,
		ECC_Visibility,
		Params
	);

	return Hit.bBlockingHit
		? Hit.ImpactPoint
		: Hit.TraceEnd;
}

void AShooterCharacter::AddWeaponClass(
	const TSubclassOf<AShooterWeapon>& WeaponClass)
{
	if (!WeaponClass)
	{
		return;
	}

	AShooterWeapon* ExistingWeapon =
		FindWeaponOfType(WeaponClass);

	if (ExistingWeapon)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AShooterWeapon* AddedWeapon =
		GetWorld()->SpawnActor<AShooterWeapon>(
			WeaponClass,
			GetActorTransform(),
			SpawnParams
		);

	if (!AddedWeapon)
	{
		return;
	}

	OwnedWeapons.Add(AddedWeapon);

	if (CurrentWeapon)
	{
		CurrentWeapon->DeactivateWeapon();
	}

	CurrentWeapon = AddedWeapon;

	CurrentWeapon->ActivateWeapon();
}

void AShooterCharacter::OnWeaponActivated(
	AShooterWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	OnBulletCountUpdated.Broadcast(
		Weapon->GetMagazineSize(),
		Weapon->GetBulletCount()
	);
}

void AShooterCharacter::OnWeaponDeactivated(
	AShooterWeapon* Weapon)
{
}

void AShooterCharacter::OnSemiWeaponRefire()
{
}

AShooterWeapon* AShooterCharacter::FindWeaponOfType(
	TSubclassOf<AShooterWeapon> WeaponClass) const
{
	for (AShooterWeapon* Weapon : OwnedWeapons)
	{
		if (Weapon && Weapon->IsA(WeaponClass))
		{
			return Weapon;
		}
	}

	return nullptr;
}

void AShooterCharacter::SetHasWeapon(bool bNewHasWeapon)
{
	bHasWeapon = bNewHasWeapon;
}

bool AShooterCharacter::HasWeapon() const
{
	return bHasWeapon;
}

void AShooterCharacter::Die()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->DeactivateWeapon();
	}

	GetCharacterMovement()->StopMovementImmediately();

	DisableInput(nullptr);

	OnBulletCountUpdated.Broadcast(0, 0);

	BP_OnDeath();

	GetWorld()->GetTimerManager().SetTimer(
		RespawnTimer,
		this,
		&AShooterCharacter::OnRespawn,
		RespawnTime,
		false
	);
}

void AShooterCharacter::OnRespawn()
{
	Destroy();
}