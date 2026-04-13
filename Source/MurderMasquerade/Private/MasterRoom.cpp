// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterRoom.h"

// Sets default values
AMasterRoom::AMasterRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	RoomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomMesh"));
	RoomMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/DynamicMansion/MRoom.MRoom")));
	if(RootComponent) RoomMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMasterRoom::BeginPlay()
{
	Super::BeginPlay();
	
	RoomMesh->bHiddenInGame = !DefaultRoom;
}

// Called every frame
void AMasterRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

