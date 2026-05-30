// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterPickup.h"

#include "ShooterWeaponHolder.h"
#include "ShooterWeapon.h"
#include "ShooterCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

#include "TimerManager.h"

AShooterPickup::AShooterPickup()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent =
        CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    SphereCollision =
        CreateDefaultSubobject<USphereComponent>(
            TEXT("SphereCollision"));

    SphereCollision->SetupAttachment(RootComponent);

    SphereCollision->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly);

    SphereCollision->SetCollisionResponseToAllChannels(
        ECR_Ignore);

    SphereCollision->SetCollisionResponseToChannel(
        ECC_Pawn,
        ECR_Overlap);

    SphereCollision->OnComponentBeginOverlap.AddDynamic(
        this,
        &AShooterPickup::OnOverlap
    );

    Mesh =
        CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    Mesh->SetupAttachment(SphereCollision);

    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShooterPickup::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (FWeaponTableRow* WeaponData =
        WeaponType.GetRow<FWeaponTableRow>(TEXT("")))
    {
        Mesh->SetStaticMesh(
            WeaponData->StaticMesh.LoadSynchronous()
        );
    }
}

void AShooterPickup::BeginPlay()
{
    Super::BeginPlay();

    if (FWeaponTableRow* WeaponData =
        WeaponType.GetRow<FWeaponTableRow>(TEXT("")))
    {
        WeaponClass = WeaponData->WeaponToSpawn;
    }
}

void AShooterPickup::EndPlay(
    const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void AShooterPickup::OnOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (IShooterWeaponHolder* WeaponHolder =
        Cast<IShooterWeaponHolder>(OtherActor))
    {
        WeaponHolder->AddWeaponClass(WeaponClass);

        if (AShooterCharacter* Character =
            Cast<AShooterCharacter>(OtherActor))
        {
            Character->SetHasWeapon(true);
        }

        SetActorHiddenInGame(true);

        SetActorEnableCollision(false);

        SetActorTickEnabled(false);

        GetWorld()->GetTimerManager().SetTimer(
            RespawnTimer,
            this,
            &AShooterPickup::RespawnPickup,
            RespawnTime,
            false
        );
    }
}

void AShooterPickup::RespawnPickup()
{
    SetActorHiddenInGame(false);

    BP_OnRespawn();
}

void AShooterPickup::FinishRespawn()
{
    SetActorEnableCollision(true);

    SetActorTickEnabled(true);
}