// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInterationComponent.h"
#include <Camera/CameraComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/Character.h>
#include "ItemInteractionInterface.h"
#include <Camera/CameraComponent.h>


// Sets default values for this component's properties
UItemInterationComponent::UItemInterationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Gets reference to Owner, PlayerCharacter in this case
	Owner = Cast<APlayerCharacter>(GetOwner());
}


// Called when the game starts
void UItemInterationComponent::BeginPlay()
{
	Super::BeginPlay();

	//Attaches to component only in memory
	AttachPoint = NewObject<USceneComponent>(Owner, USceneComponent::StaticClass(), TEXT("ItemAttachPoint"));
	AttachPoint->RegisterComponent();
	AttachPoint->AttachToComponent(Owner->GetRootComponent(),FAttachmentTransformRules::SnapToTargetIncludingScale);

	if (Owner)
	{
		Owner->InteractDelegate.AddDynamic(this, &UItemInterationComponent::PickupInteractable);
		Owner->DropItemDelegate.AddDynamic(this, &UItemInterationComponent::DropInteractable);
		Owner->PrimaryFireDelegate.AddDynamic(this, &UItemInterationComponent::UseHeldItem);
	}
}


// Called every frame
void UItemInterationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FHitResult UItemInterationComponent::LineTraceViaPlayer(FName Profile,float Range)
{
	FHitResult HitActor;
	if (Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner = true"));
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(Owner);
		if (bIsHoldingItem)IgnoreActors.Add(HeldActor);
		UCameraComponent* Camera = Owner->FindComponentByClass<UCameraComponent>();
		if (Camera)
		{
			UE_LOG(LogTemp, Warning, TEXT("Camera = true"));
			FVector Start = Camera->GetComponentLocation();
			FVector End = Start + Camera->GetForwardVector() * Range;

			UKismetSystemLibrary::LineTraceSingleByProfile(this, Start, End, Profile, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitActor, true);
		}
	}
	return HitActor;
}

void UItemInterationComponent::PickupInteractable()
{

	if (!HeldActor)
	{
		FHitResult HitResult = LineTraceViaPlayer(FName("Interactable"), PickupRange);
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Received Broadcast Interact"));
		//checks if hitactor is valid and implements interaction interface
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UItemInteractionInterface::StaticClass()))
		{
			IItemInteractionInterface::Execute_PickUpItem(HitActor, Owner);
			HeldActor = HitActor;
			if (IsValid(HeldActor)) bIsHoldingItem = true;
			
		}
	}
}

void UItemInterationComponent::DropInteractable()
{
	if (bIsHoldingItem && HeldActor) {
		FHitResult HitResult = LineTraceViaPlayer(FName("BlockAll"), PickupRange);
		if (!HitResult.bBlockingHit) return;
		FVector HitLocation = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z + 20.f);
		if (HeldActor->GetClass()->ImplementsInterface(UItemInteractionInterface::StaticClass()))
		{
			
			HeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			HeldActor->SetActorLocation(HitLocation);
			IItemInteractionInterface::Execute_DropItem(HeldActor, Owner);
			
			HeldActor = nullptr;
			if(!IsValid(HeldActor)) bIsHoldingItem = false;
		}
	}
}

void UItemInterationComponent::UseHeldItem()
{
	if (IsValid(HeldActor))
	{
		if (HeldActor->GetClass()->ImplementsInterface(UItemInteractionInterface::StaticClass()))
		{
			IItemInteractionInterface::Execute_UseItem(HeldActor, Owner);
		}
	}
}
