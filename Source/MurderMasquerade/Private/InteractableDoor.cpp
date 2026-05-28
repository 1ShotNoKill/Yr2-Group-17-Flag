// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"




AInteractableDoor::AInteractableDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultMeshPath = "/Game/Assets/DynamicMansion/MansionDblDoor.MansionDblDoor";
	BCanPickup = false;
	Center = CreateDefaultSubobject<USceneComponent>(TEXT("Center"));
	RootComponent = Center;
	
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeRotation(FRotator(0, 0, 0));
	Mesh->SetRelativeLocation(FVector(0, 0, -1000));
	

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetRelativeLocation(FVector(-0.045723, -151.754395, 0));
	RightDoor->SetRelativeRotation(FRotator(0, 180, 0));
	RightDoor->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *DefaultMeshPath));
	RightDoor->SetupAttachment(Center);

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetRelativeLocation(FVector(0, 0, 0));
	LeftDoor->SetRelativeRotation(FRotator(0, 0, 0));
	LeftDoor->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *DefaultMeshPath));
	LeftDoor->SetupAttachment(Center);
}

void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();
	StartDegree = LeftDoor->GetComponentRotation();
	Mesh->SetSimulatePhysics(false);
}

void AInteractableDoor::PickUpItem_Implementation(APlayerCharacter* Interactor)
{
	BDoorOpen = true;
	
	//ToggleDoor();
}



void AInteractableDoor::OpenDoor()
{

}

void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BDoorOpen == true)
	{
		LeftDoor->SetRelativeRotation(FMath::RInterpTo(LeftDoor->GetRelativeRotation(), FRotator(0, -TargetDegree, 0), GetWorld()->DeltaTimeSeconds, DoorSpeed));
		RightDoor->SetRelativeRotation(FMath::RInterpTo(RightDoor->GetRelativeRotation(), FRotator(0, 180+TargetDegree, 0), GetWorld()->DeltaTimeSeconds, DoorSpeed));
		//Mesh->SetRelativeRotation(Return);
		
		if (LeftDoor->GetRelativeRotation().Yaw <= -TargetDegree+1)
		{
			BDoorOpen = false;
		}


	}
}


