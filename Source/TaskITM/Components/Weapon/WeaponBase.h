#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USceneComponent;

UCLASS()
class AWeaponBase : public AActor
{
    GENERATED_BODY()
public:
                            AWeaponBase();

    /// Functions
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                    Initialize();
    virtual void            Initialize_Implementation();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                    PreDestroy();
    virtual void            PreDestroy_Implementation();
    UFUNCTION(BlueprintCallable)
    virtual void            StartAttack();
    UFUNCTION(BlueprintCallable)
    virtual void            EndAttack();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                    OnAttack(bool bEnable);
    virtual void            OnAttack_Implementation(bool bEnable);
    UFUNCTION(BlueprintPure)
    virtual bool            IsAttack() const;
    virtual void            OnTimerWeapon();
    void                    PauseTimerWeapon(bool bEnable);
    /// Variables
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float                   Range;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName                   WeaponID;

protected:
    /// Functions
    virtual void            StartTimer(float InFirstDelay = -1.f) {}
    virtual void            EndTimer() {}
    /// Variables
    UPROPERTY(BlueprintReadOnly)
    uint8                   bNeedAttack : 1;
    UPROPERTY(BlueprintReadOnly)
    uint8                   bNeedReload : 1;
    FTimerHandle            TimerHandler;
};
