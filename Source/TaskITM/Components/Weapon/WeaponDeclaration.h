#pragma once

#include "CoreMinimal.h"
#include "WeaponDeclaration.generated.h"

USTRUCT(BlueprintType)
struct FWeaponCharacteristics
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float           Damage;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float           RoundsPerMinute;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float           ClipSize;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float           ReloadTime;
};
