// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoisonZone.generated.h"

class UBoxComponent;

UCLASS()
class M_M_API APoisonZone : public AActor
{
	GENERATED_BODY()

public:
	APoisonZone();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Poison", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* PoisonArea;

	UPROPERTY(EditAnywhere, Category = "Poison")
	float DamagePerSecond = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Poison")
	float TickRate = 1.0f;

	FTimerHandle DamageTimer;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	void ApplyDamage();
};
