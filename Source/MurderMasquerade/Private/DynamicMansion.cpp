// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicMansion.h"
#include "AssetRegistry/AssetRegistryModule.h"

// Sets default values
ADynamicMansion::ADynamicMansion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	FrontDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontDoor"));
	FrontDoor->SetupAttachment(RootComponent);

	FrontDoor->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Game/Assets/DynamicMansion/FrontHallway.FrontHallway")));
}

// Called when the game starts or when spawned
void ADynamicMansion::BeginPlay()
{
	Super::BeginPlay();
	OnConstruction(GetActorTransform());
}



FVector ADynamicMansion::GetLastSegmentPosition(float Offset)
{
	if (HallwaySegments.IsEmpty()) return FVector(0,0,0);
	UChildActorComponent* LastSegment = HallwaySegments[HallwaySegments.Num() - 1]; //Gets last segment in array
	FVector LastSegPos = LastSegment->GetRelativeLocation();
	if(Offset > 0) LastSegPos.X -= SpacerOffset;
		return LastSegPos;
}

// Called every frame
void ADynamicMansion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FAssetData> ADynamicMansion::FindAssets(FName Directory)
{
	TArray<FAssetData> FoundAssets;

	FAssetRegistryModule& AssetRegModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetReg = AssetRegModule.Get();
	AssetReg.GetAssetsByPath(Directory, FoundAssets, true);
		return FoundAssets;
}

void ADynamicMansion::OnConstruction(const FTransform& Transform)
{
	//Gets all HallwayPrefabs within specified Directory
	HallwayPrefabs.Empty();
	TArray<FAssetData> Assets = FindAssets(FName("/Game/Assets/DynamicMansion/Blueprints/Hallway"));
		for (auto Asset : Assets)
		{
			HallwayPrefabs.Add(TSoftClassPtr<AMasterHallwaySegment>(Asset.ObjectPath.ToString() + TEXT("_C")));
		}
	//Gets all SpacerPrefabs within specified directory
	SpacerPrefabs.Empty();
	Assets.Empty();
	Assets = FindAssets("/Game/Assets/DynamicMansion/Blueprints/HallwaySpacer");
		for (auto Asset : Assets)
		{
			SpacerPrefabs.Add(TSoftClassPtr<AMasterSpacerSegment>(Asset.ObjectPath.ToString() + TEXT("_C")));
		}
	//Gets all RoomPrefabs within specified directory
	RoomPrefabs.Empty();
	Assets.Empty();
	Assets = FindAssets("/Game/Assets/DynamicMansion/Blueprints/Room");
		for (auto Asset : Assets)
		{
			RoomPrefabs.Add(TSoftClassPtr<AMasterRoom>(Asset.ObjectPath.ToString() + TEXT("_C")));
		}
		//Gets all Endcap Prefabs within directory
	EndCapPrefabs.Empty();
	Assets.Empty();
	Assets = FindAssets("/Game/Assets/DynamicMansion/Blueprints/EndCap");
		for (auto Asset : Assets)
		{
			EndCapPrefabs.Add(TSoftClassPtr<AMasterEndCap>(Asset.ObjectPath.ToString() + TEXT("_C")));
		}

	//Safety Checks incase arrays are empty
	if (HallwayPrefabs.Num() < 1) return;
	if (SpacerPrefabs.Num() < 1) return;
	if (RoomPrefabs.Num() < 1) return;

	//clear generated segments
	if (HallwaySegments.Num()-1 > 0)
	{
		for (auto Segment : HallwaySegments)
		{
			if(IsValid(Segment)) Segment->DestroyComponent();
		}
		HallwaySegments.Empty();
	}
	if (Rooms.Num()-1 > 0)
	{
		for (auto Room : Rooms)
		{
			if (IsValid(Room)) Room->DestroyComponent();
		}
		Rooms.Empty();
	}
	//force defaults
	if (HallLength <= 0)
	{
		HallLength = 1;
	}

	
	int index;


	for (int i = 0; i < HallLength; i++)
	{
			//Generates Spacer before Hallway Door
			for (int H = 0; H < HallspacerStart; H++)
			{
				UChildActorComponent* HallwaySpacer = NewObject<UChildActorComponent>(this);
				HallwaySpacer->RegisterComponent();

				index = FMath::RandRange(0, SpacerPrefabs.Num() - 1);
				HallwaySpacer->SetChildActorClass(SpacerPrefabs[index].LoadSynchronous());

				HallwaySpacer->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); //Attach to rootcomponent
				HallwaySpacer->SetRelativeLocation(GetLastSegmentPosition(SpacerOffset)); //Set position offset by SpacerOffset Variable
				HallwaySegments.Add(HallwaySpacer); //Add to Segment Array
			}

		//Create Hallway Doorway#
			UChildActorComponent* Hallway = NewObject<UChildActorComponent>(this);
			Hallway->RegisterComponent();

			index = FMath::RandRange(0, HallwayPrefabs.Num() - 1);
			Hallway->SetChildActorClass(HallwayPrefabs[index].LoadSynchronous());

			Hallway->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); //Attach to rootcomponent
			Hallway->SetRelativeLocation(GetLastSegmentPosition(HallwayOffset)); //Set position offset by HallwayOffset Variable
			HallwaySegments.Add(Hallway); //Add to Segment Array
		//Generate Rooms
			for (int R = 0; R < 2; R++)
			{
				UChildActorComponent* Sideroom = NewObject<UChildActorComponent>(this);
				Sideroom->RegisterComponent();

				index = FMath::RandRange(0, RoomPrefabs.Num() - 1);
				Sideroom->SetChildActorClass(RoomPrefabs[index].LoadSynchronous());

				
				Sideroom->SetRelativeLocation(GetLastSegmentPosition(0));
				Sideroom->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

				if (R == 1)
				{
					Sideroom->SetRelativeScale3D(FVector(1, -1, 1));
				}
				
				Rooms.Add(Sideroom);
			}
		
		//Generate Spacer After Hallway Door
			for (int H = 0; H < HallspacerEnd; H++)
			{
				UChildActorComponent* HallwaySpacer = NewObject<UChildActorComponent>(this);
				HallwaySpacer->RegisterComponent();

				index = FMath::RandRange(0, SpacerPrefabs.Num() - 1);
				HallwaySpacer->SetChildActorClass(SpacerPrefabs[index].LoadSynchronous());

				HallwaySpacer->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); //Attach to rootcomponent
				HallwaySpacer->SetRelativeLocation(GetLastSegmentPosition(SpacerOffset)); //Set position offset by SpacerOffset Variable
				HallwaySegments.Add(HallwaySpacer); //Add to Segment Array
			}
	}
	//Create Hallway Endcap
		UChildActorComponent* EndCap = NewObject<UChildActorComponent>(this);
		EndCap->RegisterComponent();
		index = FMath::RandRange(0, EndCapPrefabs.Num() - 1);
		EndCap->SetChildActorClass(EndCapPrefabs[index].LoadSynchronous());
		EndCap->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		EndCap->SetRelativeLocation(GetLastSegmentPosition(SpacerOffset));
		HallwaySegments.Add(EndCap);	
}

