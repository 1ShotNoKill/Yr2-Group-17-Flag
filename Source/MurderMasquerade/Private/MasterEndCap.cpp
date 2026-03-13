// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterEndCap.h"

// Sets default values
AMasterEndCap::AMasterEndCap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	EndCap = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndcapMesh"));
	EndCap->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/DynamicMansion/Endcap.Endcap")));
	if (RootComponent) EndCap->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMasterEndCap::BeginPlay()
{
	Super::BeginPlay();
	
	EndCap->bHiddenInGame = !DefaultRoom;
}

// Called every frame
void AMasterEndCap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

