// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <Kismet/KismetSystemLibrary.h>


#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	//If Context is not set in editor then load the context at path
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> Context(TEXT("/Game/Player/Input/PlayerContext_IMC.PlayerContext_IMC"));
	if (Context.Succeeded())
	{
		PlayerMappingContext = Context.Object;
	}
	//ItemComponentCreation
	ItemComponent = CreateDefaultSubobject<UItemInterationComponent>(TEXT("ItemInteractionComponent"));
	ItemComponent->RegisterComponent();

	CreateCamera();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ItemComponent)
	{
		USceneComponent* AttachPoint = ItemComponent->AttachPoint;
		AttachPoint->SetupAttachment(GetMesh());
	}
}

void APlayerCharacter::CreateCamera()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;
	FVector Location = SpringArm->GetRelativeLocation();
	Location = FVector(Location.X, Location.Y, 92.f);
	SpringArm->SetRelativeLocation(Location);


	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->bUsePawnControlRotation = false;


}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D Vector = Value.Get<FVector2D>();
	float VertScale = 1.0f;
	AddControllerPitchInput(Vector.Y * VertScale);

	float HoriScale = 1.0f;
	AddControllerYawInput(Vector.X * HoriScale);


}

void APlayerCharacter::Move(const FInputActionValue& Value)
{

}

void APlayerCharacter::Interact() 
{
	if (ItemComponent)	InteractDelegate.Broadcast();
}

void APlayerCharacter::DropItem()
{
	if (ItemComponent) DropItemDelegate.Broadcast();
}

void APlayerCharacter::PrimaryFire()
{
	PrimaryFireDelegate.Broadcast();
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = GetController<APlayerController>())
	{
		//Gets the local players subsystem via controller
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			//Adds mapping context to local players subsystem
			SubSystem->AddMappingContext(PlayerMappingContext, 0);

		}	
	}
	for (const FEnhancedActionKeyMapping& CurrentMapping : PlayerMappingContext->GetMappings())
	{
		if (CurrentMapping.Action) //if current mapping has action tied to it
		{
			FName MapName = CurrentMapping.Action->GetFName();
			const UInputAction* Action = CurrentMapping.Action.Get();
			InputActions.Add(MapName, Action);
		}
	}
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) //Binds Input actions to functions
	{
		if (InputActions.Contains("Look_IA"))		EnhancedInputComponent->BindAction(InputActions["Look_IA"], ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		if (InputActions.Contains("Interact_IA"))	EnhancedInputComponent->BindAction(InputActions["Interact_IA"], ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		if (InputActions.Contains("DropItem_IA"))	EnhancedInputComponent->BindAction(InputActions["DropItem_IA"], ETriggerEvent::Started, this, &APlayerCharacter::DropItem);
		if (InputActions.Contains("Fire_IA"))	EnhancedInputComponent->BindAction(InputActions["Fire_IA"], ETriggerEvent::Started, this, &APlayerCharacter::PrimaryFire);
	}

}

