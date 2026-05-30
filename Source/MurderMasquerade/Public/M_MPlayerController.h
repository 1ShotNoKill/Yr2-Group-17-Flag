#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "M_MPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

UCLASS()
class MURDERMASQUERADE_API AM_MPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AM_MPlayerController();

protected:

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	TObjectPtr<UUserWidget> MobileControlsWidget;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};