// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterWeapon.h"

#include "ShooterProjectile.h"
#include "ShooterWeaponHolder.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

AShooterWeapon::AShooterWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent =
        CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    FirstPersonMesh =
        CreateDefaultSubobject<USkeletalMeshComponent>(
            TEXT("FirstPersonMesh"));

    FirstPersonMesh->SetupAttachment(RootComponent);

    ThirdPersonMesh =
        CreateDefaultSubobject<USkeletalMeshComponent>(
            TEXT("ThirdPersonMesh"));

    ThirdPersonMesh->SetupAttachment(RootComponent);
}

void AShooterWeapon::BeginPlay()
{
    Super::BeginPlay();

    WeaponOwner = Cast<IShooterWeaponHolder>(GetOwner());

    CurrentBullets = MagazineSize;

    if (WeaponOwner)
    {
        WeaponOwner->AttachWeaponMeshes(this);
    }
}

void AShooterWeapon::ActivateWeapon()
{
    SetActorHiddenInGame(false);

    if (WeaponOwner)
    {
        WeaponOwner->OnWeaponActivated(this);
    }
}

void AShooterWeapon::DeactivateWeapon()
{
    StopFiring();

    SetActorHiddenInGame(true);

    if (WeaponOwner)
    {
        WeaponOwner->OnWeaponDeactivated(this);
    }
}

void AShooterWeapon::StartFiring()
{
    Fire();
}

void AShooterWeapon::StopFiring()
{
}

void AShooterWeapon::Fire()
{
    if (!ProjectileClass || !WeaponOwner)
    {
        return;
    }

    FVector TargetLocation =
        WeaponOwner->GetWeaponTargetLocation();

    FVector SpawnLocation =
        FirstPersonMesh->GetSocketLocation(MuzzleSocketName);

    FRotator SpawnRotation =
        (TargetLocation - SpawnLocation).Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = Cast<APawn>(GetOwner());

    GetWorld()->SpawnActor<AShooterProjectile>(
        ProjectileClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    WeaponOwner->AddWeaponRecoil(FiringRecoil);

    CurrentBullets--;

    if (CurrentBullets <= 0)
    {
        CurrentBullets = MagazineSize;
    }

    WeaponOwner->UpdateWeaponHUD(
        CurrentBullets,
        MagazineSize
    );
}