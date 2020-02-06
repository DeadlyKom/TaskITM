#pragma once

#include "CoreMinimal.h"
#include "TaskITM/Characters/CharacterBase.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class APlayerCharacterBase : public ACharacterBase
{
    GENERATED_BODY()
public:
                                    APlayerCharacterBase();
    /// Functions
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void                            SetCharacterSelection(bool bEnable);
    virtual void                    SetCharacterSelection_Implementation(bool bEnable);
};
