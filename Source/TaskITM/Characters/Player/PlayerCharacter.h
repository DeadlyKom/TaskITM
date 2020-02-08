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
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                            SetCharacterSelection(bool bEnable);
    virtual void                    SetCharacterSelection_Implementation(bool bEnable);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                            TurnWeaponToNearTarget(AActor* NearTarget);
    virtual void                    TurnWeaponToNearTarget_Implementation(AActor* NearTarget);

protected:
    /// Override functions
    virtual void                    Tick(float DeltaSeconds) override;
};
