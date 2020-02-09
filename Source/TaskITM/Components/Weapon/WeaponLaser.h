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
    /// Functions
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                    ActivateLaser();
    virtual void            ActivateLaser_Implementation();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                    ReloadEnergy();
    virtual void            ReloadEnergy_Implementation();
    /// Override functions
    virtual void            Initialize_Implementation();
    /// Variables
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   DamagePerSecond;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   WastEnergyPerSecond;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   EnergyElementSize;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   ReloadTime;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                   TimeStep;

protected:
    /// Override functions
    virtual void            OnTimerWeapon() override;
    virtual void            StartTimer(float InFirstDelay = -1.f) override;
    virtual void            EndTimer() override;
    /// Variables
    UPROPERTY(BlueprintReadOnly)
    float                   EnergyElement;
};
