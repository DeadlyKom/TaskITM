#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponAutomaticRifle.generated.h"

UCLASS(Blueprintable)
class AWeaponAutomaticRifle final : public AWeaponBase
{
    GENERATED_BODY()
public:
                            AWeaponAutomaticRifle();
    /// Variables
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   Damage;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   RoundsPerMinute;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   ClipSize;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   ReloadTime;

protected:
    /// Functions
    virtual void            StartTimer(float InFirstDelay = -1.f) override;
    virtual void            EndTimer() override;
};
