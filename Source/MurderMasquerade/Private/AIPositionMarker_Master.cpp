// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPositionMarker_Master.h"
#include "WS_AISpawner.h"

class UWS_AISpawner;

// Sets default values
AAIPositionMarker_Master::AAIPositionMarker_Master()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MarkersParent.Add(nullptr);
	FocusTarget = nullptr;
}

void AAIPositionMarker_Master::OnConstruction(const FTransform& Transform)
{
	for (int i = 0; i<MarkersParent.Num();i++)
	{
		if (!MarkersParent[i])
		{
			UChildActorComponent* MarkerParent = NewObject<UChildActorComponent>(this);
			MarkerParent->SetChildActorClass(AAIMarker_Actor::StaticClass());
			MarkerParent->SetupAttachment(RootComponent);
			MarkerParent->RegisterComponent();

			MarkersParent[i] = MarkerParent;
		}
	}
}

TArray<AActor*> AAIPositionMarker_Master::GetAllMarkerActors()
{
	TArray<AActor*> MarkerActors;
	for (auto Parent : MarkersParent)
	{
		MarkerActors.Add(Parent->GetChildActor());
	}
	return MarkerActors;
}

// Called when the game starts or when spawned
void AAIPositionMarker_Master::BeginPlay()
{
	Super::BeginPlay();
	
	//TArray<AActor*> Markers = GetAllMarkerActors();
	//UWS_AISpawner* Spawner = GetWorld()->GetSubsystem<UWS_AISpawner>();
	//if (!Spawner) return;
	//for (auto Marker : Markers)
	//{
	//	Spawner->MarkerActors.Add(Marker);
	//}
}

// Called every frame
void AAIPositionMarker_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

