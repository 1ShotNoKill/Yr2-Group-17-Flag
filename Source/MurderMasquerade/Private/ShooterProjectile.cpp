// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Character.h"

AShooterProjectile::AShooterProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent =
        CreateDefaultSubobject<USphereComponent>(
            TEXT("CollisionComponent"));

    RootComponent = CollisionComponent;

    CollisionComponent->SetSphereRadius(16.0f);

    CollisionComponent->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics);

    ProjectileMovement =
        CreateDefaultSubobject<UProjectileMovementComponent>(
            TEXT("ProjectileMovement"));

    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;

    HitDamageType = UDamageType::StaticClass();
}

void AShooterProjectile::BeginPlay()
{
    Super::BeginPlay();

    CollisionComponent->IgnoreActorWhenMoving(
        GetInstigator(),
        true
    );
}

void AShooterProjectile::NotifyHit(
    UPrimitiveComponent* MyComp,
    AActor* Other,
    UPrimitiveComponent* OtherComp,
    bool bSelfMoved,
    FVector HitLocation,
    FVector HitNormal,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (ACharacter* HitCharacter = Cast<ACharacter>(Other))
    {
        if (HitCharacter != GetOwner() || bDamageOwner)
        {
            UGameplayStatics::ApplyDamage(
                HitCharacter,
                HitDamage,
                GetInstigatorController(),
                this,
                HitDamageType
            );
        }
    }

    if (OtherComp && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(
            GetVelocity() * PhysicsForce,
            HitLocation
        );
    }

    Destroy();
}