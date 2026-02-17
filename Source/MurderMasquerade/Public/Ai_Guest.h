// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Master_Ai.h"
#include "Ai_Guest.generated.h"

/**
 * 
 */
UCLASS()
class MURDERMASQUERADE_API AAi_Guest : public AMaster_Ai
{
	GENERATED_BODY()
	AAi_Guest();

public:
	void SetMask(UStaticMesh* Mesh);


private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mask;
	void CreateMaskComponent();
};
