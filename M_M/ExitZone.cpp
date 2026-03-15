#include "ExitZone.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameMode.h"

AExitZone::AExitZone()
{
	PrimaryActorTick.bCanEverTick = false;

	ExitTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitTrigger"));
	RootComponent = ExitTrigger;

	ExitTrigger->SetCollisionProfileName(TEXT("Trigger"));
}

void AExitZone::BeginPlay()
{
	Super::BeginPlay();

	ExitTrigger->OnComponentBeginOverlap.AddDynamic(
		this,
		&AExitZone::OnOverlapBegin
	);

	ExitTrigger->OnComponentEndOverlap.AddDynamic(
		this,
		&AExitZone::OnOverlapEnd
	);
}

void AExitZone::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor)
		return;

	if (!OtherActor->IsA(ACharacter::StaticClass()))
		return;

	// If the player re-enters after leaving, trigger win
	if (bPlayerWasInside)
	{
		AShooterGameMode* GM = Cast<AShooterGameMode>(
			UGameplayStatics::GetGameMode(GetWorld())
		);

		if (GM && GM->IsEscapeActive())
		{
			GM->PlayerEscaped();
		}
	}
}

void AExitZone::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	if (!OtherActor)
		return;

	if (!OtherActor->IsA(ACharacter::StaticClass()))
		return;

	// Player has left the zone
	bPlayerWasInside = true;
}
