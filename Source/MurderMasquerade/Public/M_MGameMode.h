#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "M_MGameMode.generated.h"

UCLASS()
class MURDERMASQUERADE_API AM_MGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:

    AM_MGameMode();

protected:

    virtual void BeginPlay() override;
};