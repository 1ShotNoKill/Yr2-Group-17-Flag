// Fill out your copyright notice in the Description page of Project Settings.

#include "WS_AISpawner.h"
#include <Kismet/GameplayStatics.h>
#include <Ai_Guest.h>


UWS_AISpawner::UWS_AISpawner()
{
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

AAIMarker_Actor* UWS_AISpawner::GetRandomMarker()
{
	if (AllMarkers.Num() > 0)
	{
		float RN = FMath::RandRange(0, AllMarkers.Num() - 1);
		AActor* CurrentMarker = AllMarkers[RN];
		AAIMarker_Actor* SelectedMarker = Cast<AAIMarker_Actor>(CurrentMarker);
		if (SelectedMarker && SelectedMarker->bIsAvaliable == true) return SelectedMarker;
	}
	return nullptr;
}

void UWS_AISpawner::FindAllMarkerActors()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIMarker_Actor::StaticClass(), AllMarkers);
	int Num = AllMarkers.Num();
}

void UWS_AISpawner::SpawnGuests()
{
	FindAllMarkerActors();

	if (AllMarkers.Num() <= 0) return;
	
	TArray<AActor*> LocalAllMarkers = AllMarkers;
	for (int i = 0; i < MaxGuests -1; i++)
	{
		int RandomIndex = FMath::RandRange(0, LocalAllMarkers.Num()-1);
		AActor* Marker;
		if (LocalAllMarkers[RandomIndex])
		{
			Marker = LocalAllMarkers[RandomIndex];
		}
		else Marker = LocalAllMarkers[0];


		//get random marker to use as spawn location for guest npc
		AAi_Guest* Guest = GetWorld()->SpawnActor<AAi_Guest>(AAi_Guest::StaticClass(), Marker->GetActorLocation(), Marker->GetActorRotation(), SpawnParams); //Spawns guest at random marker

		EMask RandomMask = GetRandomMask(); //Get Random Mask Key


		FString Path = MaskMap[RandomMask]; //Get Value at key
		UStaticMesh* GuestMask = LoadObject<UStaticMesh>(nullptr, *Path); //Load object using path from randommask()
		if (GuestMask) Guest->SetMask(GuestMask); //if valid sets mask for guest

		if (const UEnum* Enum = StaticEnum<EMask>()) //Get display name from enum key
		{
			FText MaskName = Enum->GetDisplayNameTextByValue(static_cast<uint8>(RandomMask));
			FName TagName = (*MaskName.ToString()); //Convert FText to FName
			Guest->Tags.Add(TagName); //Adds mask name to guest npc
		}		
		if (RandomMask == TargetMask)
		{
			Guest->Tags.Add("Target"); //If guest wears target mark then mark then as the target.
			Guest->bIsTarget = true;
			Target = Guest;

			AMurderGameMode* GM = Cast<AMurderGameMode>(GetWorld()->GetAuthGameMode());
			if(GM) Guest->OnDeath.AddDynamic(GM, &AMurderGameMode::OnTargetDeath);
			UE_LOG(LogTemp, Warning, TEXT("Bound to Target"));
		}
		Guest->CurrentMaskType = RandomMask;
		

		LocalAllMarkers.RemoveAt(RandomIndex);
	}
}

EMask UWS_AISpawner::GetRandomMask()
{
	TArray<EMask> Keys;
	MaskMap.GenerateKeyArray(Keys);
	Keys.Remove(EMask::None);

	if (TargetMask == EMask::None) //if Mask is not set then return random mask as targets;
	{
		EMask TargetType = Keys[FMath::RandRange(0, Keys.Num() - 1)];
		TargetMask = TargetType;
		return TargetType;
	}
	else
	{
		Keys.Remove(TargetMask);
		EMask Type = Keys[FMath::RandRange(0, Keys.Num() - 1)];
		return Type;
	}
	return EMask::None;
}

AActor* UWS_AISpawner::GetTarget()
{
	return Target;
}

void UWS_AISpawner::OnWorldBeginPlay(UWorld& World)
{
	MaskMap.Add(EMask::Bird, "/Game/Masks/BirdMask.BirdMask");
	MaskMap.Add(EMask::Bear, "/Game/Masks/BearMask.BearMask");
	MaskMap.Add(EMask::Rabbit, "/Game/Masks/RabbitMask.RabbitMask");


	FTimerHandle SpawnTimer;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer,this, &UWS_AISpawner::SpawnGuests, 1.f, false);

}
