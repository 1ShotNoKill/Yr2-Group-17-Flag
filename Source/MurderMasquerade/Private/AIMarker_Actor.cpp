// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMarker_Actor.h"
#include "Components/ArrowComponent.h"

// Sets default values
AAIMarker_Actor::AAIMarker_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create Arrow component

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
		Base->SetupAttachment(RootComponent);
		Base->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Ai/Ai_Marker/ControlRig_Circle_solid.ControlRig_Circle_solid")));
		Base->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Base->bHiddenInGame = true;

	UArrowComponent* Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
		Arrow->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
		if(RootComponent) Arrow->SetupAttachment(Base);

	
}

// Called when the game starts or when spawned
void AAIMarker_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIMarker_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

