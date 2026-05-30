// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterWeapon.generated.h"

class AShooterProjectile;
class USkeletalMeshComponent;
class UAnimMontage;
class UAnimInstance;
class IShooterWeaponHolder;

UCLASS(Abstract)
class MURDERMASQUERADE_API AShooterWeapon : public AActor
{
    GENERATED_BODY()

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* FirstPersonMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* ThirdPersonMesh;

protected:

    IShooterWeaponHolder* WeaponOwner;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AShooterProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 MagazineSize = 10;

    int32 CurrentBullets = 0;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    UAnimMontage* FiringMontage;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    float FiringRecoil = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName MuzzleSocketName;

public:

    AShooterWeapon();

protected:

    virtual void BeginPlay() override;

public:

    void ActivateWeapon();

    void DeactivateWeapon();

    void StartFiring();

    void StopFiring();

protected:

    virtual void Fire();

public:

    USkeletalMeshComponent* GetFirstPersonMesh() const
    {
        return FirstPersonMesh;
    }

    USkeletalMeshComponent* GetThirdPersonMesh() const
    {
        return ThirdPersonMesh;
    }

    int32 GetMagazineSize() const
    {
        return MagazineSize;
    }

    int32 GetBulletCount() const
    {
        return CurrentBullets;
    }
};