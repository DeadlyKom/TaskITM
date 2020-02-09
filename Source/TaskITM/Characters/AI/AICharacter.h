#pragma once

#include "CoreMinimal.h"
#include "TaskITM/Characters/CharacterBase.h"
#include "AICharacter.generated.h"

UCLASS()
class AAICharacterBase : public ACharacterBase
{
    GENERATED_BODY()
public:
                                    AAICharacterBase();
    /// Functions
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                            TurnWeaponToNearTarget(AActor* NearTarget);
    virtual void                    TurnWeaponToNearTarget_Implementation(AActor* NearTarget);
protected:
    /// Override functions
    virtual void                    Tick(float DeltaSeconds) override;
};
