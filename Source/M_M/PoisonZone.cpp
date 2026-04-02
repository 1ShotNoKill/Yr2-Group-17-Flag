// Fill out your copyright notice in the Description page of Project Settings.

#include "PoisonZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

APoisonZone::APoisonZone()
{
	PrimaryActorTick.bCanEverTick = false;

	PoisonArea = CreateDefaultSubobject<UBoxComponent>(TEXT("PoisonArea"));
	RootComponent = PoisonArea;

	PoisonArea->SetCollisionProfileName(TEXT("Trigger"));
}

void APoisonZone::BeginPlay()
{
	Super::BeginPlay();

	PoisonArea->OnComponentBeginOverlap.AddDynamic(this, &APoisonZone::OnOverlapBegin);
	PoisonArea->OnComponentEndOverlap.AddDynamic(this, &APoisonZone::OnOverlapEnd);
}

void APoisonZone::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
		return;

	// start damage timer only when something enters
	GetWorldTimerManager().SetTimer(
		DamageTimer,
		this,
		&APoisonZone::ApplyDamage,
		TickRate,
		true
	);
}

void APoisonZone::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	// stop timer when leaving
	GetWorldTimerManager().ClearTimer(DamageTimer);
}

void APoisonZone::ApplyDamage()
{
	TArray<AActor*> OverlappingActors;
	PoisonArea->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (!IsValid(Actor))
			continue;

		UGameplayStatics::ApplyDamage(
			Actor,
			DamagePerSecond,
			nullptr,
			this,
			nullptr
		);
	}
}
