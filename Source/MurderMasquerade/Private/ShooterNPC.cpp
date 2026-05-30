// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterNPC.h"
#include "ShooterWeapon.h"
#include "ShooterGameMode.h"
#include "ShooterCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

void AShooterNPC::BeginPlay()
{
	Super::BeginPlay();

	if (!WeaponClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Weapon = GetWorld()->SpawnActor<AShooterWeapon>(
		WeaponClass,
		GetActorTransform(),
		SpawnParams
	);
}

void AShooterNPC::EndPlay(
	const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
}

float AShooterNPC::TakeDamage(
	float Damage,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	if (bIsDead)
	{
		return 0.0f;
	}

	CurrentHP -= Damage;

	if (CurrentHP <= 0.0f)
	{
		Die();
	}

	return Damage;
}

void AShooterNPC::AttachWeaponMeshes(
	AShooterWeapon* WeaponToAttach)
{
	if (!WeaponToAttach)
	{
		return;
	}

	const FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		false
	);

	WeaponToAttach->AttachToActor(this, AttachRules);

	WeaponToAttach->GetThirdPersonMesh()->AttachToComponent(
		GetMesh(),
		AttachRules,
		ThirdPersonWeaponSocket
	);
}

void AShooterNPC::PlayFiringMontage(UAnimMontage* Montage)
{
}

void AShooterNPC::AddWeaponRecoil(float Recoil)
{
}

void AShooterNPC::UpdateWeaponHUD(
	int32 CurrentAmmo,
	int32 MagazineSize)
{
}

FVector AShooterNPC::GetWeaponTargetLocation()
{
	FVector AimSource = GetActorLocation() + FVector(0, 0, 50);

	FVector AimDirection;

	if (CurrentAimTarget)
	{
		FVector AimTarget = CurrentAimTarget->GetActorLocation();

		AimTarget.Z += FMath::RandRange(
			MinAimOffsetZ,
			MaxAimOffsetZ
		);

		AimDirection =
			(AimTarget - AimSource).GetSafeNormal();

		AimDirection =
			UKismetMathLibrary::RandomUnitVectorInConeInDegrees(
				AimDirection,
				AimVarianceHalfAngle
			);
	}
	else
	{
		AimDirection = GetActorForwardVector();
	}

	FVector TraceEnd =
		AimSource + (AimDirection * AimRange);

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(
		Hit,
		AimSource,
		TraceEnd,
		ECC_Visibility,
		Params
	);

	return Hit.bBlockingHit
		? Hit.ImpactPoint
		: Hit.TraceEnd;
}

void AShooterNPC::AddWeaponClass(
	const TSubclassOf<AShooterWeapon>& InWeaponClass)
{
}

void AShooterNPC::OnWeaponActivated(
	AShooterWeapon* InWeapon)
{
}

void AShooterNPC::OnWeaponDeactivated(
	AShooterWeapon* InWeapon)
{
}

void AShooterNPC::OnSemiWeaponRefire()
{
	if (bIsShooting && Weapon)
	{
		Weapon->StartFiring();
	}
}

void AShooterNPC::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	if (AShooterGameMode* GM =
		Cast<AShooterGameMode>(
			UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->StartEscapeTimer();
	}

	OnPawnDeath.Broadcast();

	GetCapsuleComponent()->SetCollisionEnabled(
		ECollisionEnabled::NoCollision
	);

	GetCharacterMovement()->StopMovementImmediately();

	GetMesh()->SetCollisionProfileName(
		RagdollCollisionProfile
	);

	GetMesh()->SetSimulatePhysics(true);

	GetWorld()->GetTimerManager().SetTimer(
		DeathTimer,
		this,
		&AShooterNPC::DeferredDestruction,
		DeferredDestructionTime,
		false
	);
}

void AShooterNPC::DeferredDestruction()
{
	Destroy();
}

void AShooterNPC::StartShooting(
	AActor* ActorToShoot)
{
	AShooterCharacter* PlayerCharacter =
		Cast<AShooterCharacter>(ActorToShoot);

	if (PlayerCharacter &&
		!PlayerCharacter->HasWeapon())
	{
		bIsShooting = false;

		CurrentAimTarget = nullptr;

		if (Weapon)
		{
			Weapon->StopFiring();
		}

		return;
	}

	CurrentAimTarget = ActorToShoot;

	bIsShooting = true;

	if (Weapon)
	{
		Weapon->StartFiring();
	}
}

void AShooterNPC::StopShooting()
{
	bIsShooting = false;

	CurrentAimTarget = nullptr;

	if (Weapon)
	{
		Weapon->StopFiring();
	}
}