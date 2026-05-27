// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"




AInteractableDoor::AInteractableDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultMeshPath = "/Game/Assets/DynamicMansion/MansionDblDoor.MansionDblDoor";
	BCanPickup = false;
	Mesh->DetachFromParent();
	Center = CreateDefaultSubobject<USceneComponent>(TEXT("Center"));
	RootComponent = Center;
	
	Mesh->SetupAttachment(Center);
	Mesh->SetRelativeRotation(FRotator(0, 0, 0));
	Mesh->SetRelativeLocation(FVector(0, 0, 0));

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetRelativeLocation(FVector(-0.045723, -151.754395, 0));
	RightDoor->SetRelativeRotation(FRotator(0, 180, 0));
	RightDoor->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *DefaultMeshPath));
	RightDoor->SetupAttachment(Center);
}

void AInteractableDoor::BeginPlay()
{
	StartDegree = Mesh->GetRelativeRotation().Yaw;
}

void AInteractableDoor::PickUpItem_Implementation(APlayerCharacter* Interactor)
{
	BDoorOpen = true;
	
	//ToggleDoor();
}

void AInteractableDoor::ToggleDoor()
{
	//if (BDoorOpen == false)
	//{
	//	//OpenDoor();
	//	BDoorOpen = true;
	//}
}

void AInteractableDoor::OpenDoor()
{
	//if (OpenHandle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(OpenHandle); OpenHandle.Invalidate();
	//GetWorld()->GetTimerManager().SetTimer(OpenHandle,
	//	[this]()
	//	{
	//		if (IsValid(Mesh))
	//		{
	//			float Distance = Mesh->GetRelativeRotation().Yaw - TargetDegree;
	//			float Step = Distance * (GetWorld()->DeltaTimeSeconds * DoorSpeed);
	//			Mesh->SetRelativeRotation(FRotator(0, Mesh->GetRelativeRotation().Yaw - Step, 0));

	//			if (IsValid(RightDoor))
	//			{
	//				//Distance = RightDoor->GetRelativeRotation().Yaw - 180 - TargetDegree;
	//				RightDoor->SetRelativeRotation(FRotator(0, RightDoor->GetRelativeRotation().Yaw + Step, 0));
	//			}

	//			if (Distance < 0.1f)
	//			{
	//				GetWorld()->GetTimerManager().ClearTimer(OpenHandle);
	//				OpenHandle.Invalidate();
	//			}
	//		}
	//	}, GetWorld()->DeltaTimeSeconds, true);
}

void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BDoorOpen == true)
	{
			float Distance = Mesh->GetRelativeRotation().Yaw - TargetDegree;
			float Step = Distance * (GetWorld()->DeltaTimeSeconds * DoorSpeed);

			Mesh->SetRelativeRotation(FRotator(0, Mesh->GetRelativeRotation().Yaw - Step, 0));
			RightDoor->SetRelativeRotation(FRotator(0, RightDoor->GetRelativeRotation().Yaw + Step, 0));

			if (Distance < 0.1f)
			{
				BDoorOpen = false;
			}
		}
}


