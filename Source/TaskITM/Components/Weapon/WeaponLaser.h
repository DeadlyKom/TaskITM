#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponLaser.generated.h"

UCLASS(Blueprintable)
class AWeaponLaser final : public AWeaponBase
{
    GENERATED_BODY()
public:
                            AWeaponLaser();
    /// Variables
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   DamagePerSecond;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   EnergyElementSize;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   WastEnergyPerSecond;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   ReloadTime;

protected:
    /// Functions
    virtual void            OnTimerWeapon() override;
    virtual void            StartTimer(float InFirstDelay = -1.f) override;
    virtual void            EndTimer() override;
};
