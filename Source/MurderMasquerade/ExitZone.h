#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExitZone.generated.h"

class UBoxComponent;

UCLASS()
class M_M_API AExitZone : public AActor
{
	GENERATED_BODY()

public:

	AExitZone();

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ExitTrigger;

	virtual void BeginPlay() override;

	bool bPlayerWasInside = false;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
};
