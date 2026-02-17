// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai_Guest.h"

AAi_Guest::AAi_Guest()
{


	CreateMaskComponent();
}

void AAi_Guest::SetMask(UStaticMesh* MaskMesh)
{
	Mask->SetStaticMesh(MaskMesh);
}

void AAi_Guest::CreateMaskComponent()
{
	Mask = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mask Mesh"));
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (SkeletalMesh)
	{
		Mask->SetupAttachment(GetMesh(), FName("head"));
		Mask->SetStaticMesh(nullptr);
		Mask->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		FRotator Rot = Mask->GetRelativeRotation();
		FRotator NewRot = FRotator(Rot.Roll, 90, -90);
		Mask->SetRelativeRotation(NewRot);
	}

}
