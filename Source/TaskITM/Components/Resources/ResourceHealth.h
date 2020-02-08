#pragma once

#include "CoreMinimal.h"
#include "ResourceBase.h"
#include "ResourceHealth.generated.h"

UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UResourceHealth : public UResourceBase
{
    GENERATED_BODY()

public:
                        UResourceHealth();
protected:
    /// Override functions
    virtual void        BeginPlay() override;
    /// Variables
    UPROPERTY(EditDefaultsOnly)
    float               DefaultHealth;
};