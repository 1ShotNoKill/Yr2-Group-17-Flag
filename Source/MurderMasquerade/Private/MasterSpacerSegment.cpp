// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterSpacerSegment.h"

// Sets default values
AMasterSpacerSegment::AMasterSpacerSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SpacerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpacerMesh"));
	SpacerMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/DynamicMansion/HallwaySegNoDoor.HallwaySegNoDoor")));
	if (RootComponent) SpacerMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMasterSpacerSegment::BeginPlay()
{
	Super::BeginPlay();
	SpacerMesh->bHiddenInGame = !DefaultRoom;
}

// Called every frame
void AMasterSpacerSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

