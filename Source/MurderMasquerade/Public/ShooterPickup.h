// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ShooterPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AShooterWeapon;
class UPrimitiveComponent;

USTRUCT(BlueprintType)
struct FWeaponTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UStaticMesh> StaticMesh;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AShooterWeapon> WeaponToSpawn;
};

UCLASS(Abstract)
class MURDERMASQUERADE_API AShooterPickup : public AActor
{
    GENERATED_BODY()

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* SphereCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* Mesh;

protected:

    UPROPERTY(EditAnywhere, Category = "Pickup")
    FDataTableRowHandle WeaponType;

    TSubclassOf<AShooterWeapon> WeaponClass;

    UPROPERTY(EditAnywhere, Category = "Pickup")
    float RespawnTime = 4.0f;

    FTimerHandle RespawnTimer;

public:

    AShooterPickup();

protected:

    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION()
    virtual void OnOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    void RespawnPickup();

    UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
    void BP_OnRespawn();

    UFUNCTION(BlueprintCallable, Category = "Pickup")
    void FinishRespawn();
};