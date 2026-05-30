// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UPrimitiveComponent;

UCLASS(Abstract)
class MURDERMASQUERADE_API AShooterProjectile : public AActor
{
    GENERATED_BODY()

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovement;

protected:

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float PhysicsForce = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float HitDamage = 25.0f;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    TSubclassOf<UDamageType> HitDamageType;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    bool bDamageOwner = false;

public:

    AShooterProjectile();

protected:

    virtual void BeginPlay() override;

    virtual void NotifyHit(
        UPrimitiveComponent* MyComp,
        AActor* Other,
        UPrimitiveComponent* OtherComp,
        bool bSelfMoved,
        FVector HitLocation,
        FVector HitNormal,
        FVector NormalImpulse,
        const FHitResult& Hit
    ) override;
};