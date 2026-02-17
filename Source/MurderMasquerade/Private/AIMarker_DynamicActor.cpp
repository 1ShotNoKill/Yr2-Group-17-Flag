// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMarker_DynamicActor.h"

AAIMarker_DynamicActor::AAIMarker_DynamicActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->bDoCollisionTest = false;
		FocusActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FocusObject"));

		FocusActor->SetupAttachment(SpringArm);
}

void AAIMarker_DynamicActor::OnConstruction(const FTransform& Transform)
{
	if (SpringArm) SpringArm->TargetArmLength = SpringArmDistance * -1;
	if(FocusActor) FocusActor->SetStaticMesh(ObjectMesh);
}
