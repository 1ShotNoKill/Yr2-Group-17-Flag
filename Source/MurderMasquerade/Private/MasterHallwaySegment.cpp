// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterHallwaySegment.h"

// Sets default values
AMasterHallwaySegment::AMasterHallwaySegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	HallwayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HallwayMesh"));
	HallwayMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/DynamicMansion/HallwaySeg.HallwaySeg")));
	if (RootComponent) HallwayMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMasterHallwaySegment::BeginPlay()
{
	Super::BeginPlay();
	HallwayMesh->bHiddenInGame = !DefaultRoom;
}

// Called every frame
void AMasterHallwaySegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

