// Fill out your copyright notice in the Description page of Project Settings.


#include "Master_Ai.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Navigation/PathFollowingComponent.h"
#include "AIPositionMarker_Master.h"

#include "AIMarker_Actor.h"

// Sets default values
AMaster_Ai::AMaster_Ai()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ACrowdAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	USkeletalMeshComponent* CharMesh = GetMesh();
	USkeletalMesh* LoadedMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Ai/CharacterBase/AI_Temp.AI_Temp"));
	CharMesh->SetSkeletalMesh(LoadedMesh);

	FVector Location = CharMesh->GetRelativeLocation();
	Location = FVector(Location.X, Location.Y, Location.Z - 90.f);
	FRotator Rotation = CharMesh->GetRelativeRotation();
	Rotation = FRotator(Rotation.Roll, Rotation.Yaw - 90.f, Rotation.Pitch);

	CharMesh->SetRelativeLocation(Location);
	CharMesh->SetRelativeRotation(Rotation);

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

}

// Called when the game starts or when spawned
void AMaster_Ai::BeginPlay()
{
	Super::BeginPlay();

	Spawner = Cast<UWS_AISpawner>(GetWorld()->GetSubsystem<UWS_AISpawner>());

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPositionMarker_Master::StaticClass(), MarkerActors); //Gets all placed Ai_markers within world

	AIController = Cast<AAIController>(GetController()); //gets the current controller attached to this character
	NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()); //Gets the current navsystem in play

	AIController->ReceiveMoveCompleted.AddDynamic(this, &AMaster_Ai::RotateCharacter);


	FTimerHandle TimerHandle;
	if (!TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMaster_Ai::MoveToRandomMarker, FMath::RandRange(5,15), true);
	}
}

void AMaster_Ai::MoveToRandomLocation(float Range)
{
	if (NavSystem && AIController)
	{
		FNavLocation NavLoc;
		NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), 500.f, NavLoc);
		AIController->MoveToLocation(NavLoc.Location, 1.f,false,true,true,false,0,true);
	}
}

void AMaster_Ai::MoveToRandomMarker()
{
	if (!GetVelocity().Length() == 0) return;

	if (Spawner)
	{
	if (CurrentMarker) //Check if AI already has a valid marker
	{
		CurrentMarker->bIsAvaliable = true; //Restores marker to default state
		CurrentMarker = nullptr; //clears reference to marker
	}
	CurrentMarker = Spawner->GetRandomMarker(); //Requests a new marker

		if (CurrentMarker && CurrentMarker->bIsAvaliable == true)
		{
			CurrentMarker->bIsAvaliable = false;
			FVector Location = CurrentMarker->GetActorLocation();
			AIController->MoveToLocation(Location, -1.f, true, true, true, false, nullptr, true);
		}
	}
}

void AMaster_Ai::test()
{
	MoveToRandomLocation(500.f);
}

void AMaster_Ai::RotateCharacter(FAIRequestID ID,EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success && CurrentMarker)
	{
		//if (Cast<AAIPositionMarker_Master>(CurrentMarker->GetParentActor())->FocusTarget)
		//{
		//	AIController->SetFocus(Cast<AAIPositionMarker_Master>(CurrentMarker->GetParentActor())->FocusTarget);
		//}
		//else 
		//{
			FRotator Rotation = CurrentMarker->GetActorRotation();
			SetActorRotation(Rotation);
		/*}*/
	}
}

// Called every frame
void AMaster_Ai::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMaster_Ai::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
