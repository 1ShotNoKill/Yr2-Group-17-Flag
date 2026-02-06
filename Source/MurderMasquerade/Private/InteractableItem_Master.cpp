// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableItem_Master.h"
#include "ItemInterationComponent.h"

// Sets default values
AInteractableItem_Master::AInteractableItem_Master()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AInteractableItem_Master::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableItem_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableItem_Master::PickUpItem_Implementation(APlayerCharacter* Interactor)
{
	if (!Interactor) return;
	if (UItemInterationComponent* ItemComponent = Interactor->FindComponentByClass<UItemInterationComponent>())
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (ItemComponent && ItemComponent->AttachPoint) Mesh->AttachToComponent(ItemComponent->AttachPoint,FAttachmentTransformRules::SnapToTargetIncludingScale);

		else return;
	}
UE_LOG(LogTemp, Warning, TEXT("Pickup Implementation"));
}

void AInteractableItem_Master::DropItem_Implementation(APlayerCharacter* Interactor)
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetSimulatePhysics(true);
	UE_LOG(LogTemp, Warning, TEXT("Item Drop (Internal)"));
}

void AInteractableItem_Master::UseItem_Implementation(APlayerCharacter* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("UsedItem"));
}

