#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TaskITM/Interfaces/ICharacterBase.h"
#include "CharacterBase.generated.h"

class AWeaponBase;
class UResourceHealth;
class UChildActorComponent;

UCLASS(Blueprintable)
class ACharacterBase : public ACharacter, public ICharacterBaseClass
{
    GENERATED_BODY()
    using MapAllWeapons = TMap<FName, TWeakObjectPtr<UChildActorComponent>>;
public:
                                    ACharacterBase();
    /// Interface Implementation
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UResourceHealth*                GetResourceHealth() const;
    virtual UResourceHealth*        GetResourceHealth_Implementation() const override;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    AWeaponBase*                    GetWeapon(FName NameWeapon) const;
    virtual AWeaponBase*            GetWeapon_Implementation(FName NameWeapon) const override;
    /// Functions
    UFUNCTION(BlueprintCallable)
    ACharacterBase*                 SearchNearestCharacter(TSubclassOf<ACharacterBase> Class);
    UFUNCTION(BlueprintPure)
    ACharacterBase*                 GetNearestCharacter() const;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                            SetNewWeapon(UChildActorComponent* ChildActorComponent, TSubclassOf<AWeaponBase> NewWeaponClass);
    virtual void                    SetNewWeapon_Implementation(UChildActorComponent* ChildActorComponent, TSubclassOf<AWeaponBase> NewWeaponClass);
    UFUNCTION(BlueprintNativeEvent)
    void                            PreDestroy();
    virtual void                    PreDestroy_Implementation();
    /// Variables
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                           VisionRange;
    UPROPERTY(BlueprintReadOnly)
    uint8                           bCallPreDestroy : 1;

protected:
    /// Override functions
    virtual void                    BeginPlay() override;
    virtual void                    EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    /// Handler Events
    UFUNCTION(BlueprintNativeEvent)
    void                            OnResourceMinValueHitEvent();
    virtual void                    OnResourceMinValueHitEvent_Implementation();
    UFUNCTION(BlueprintNativeEvent)
    void                            OnTakeAnyDamageEvent(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
    virtual void                    OnTakeAnyDamageEvent_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
    /// Variables
    UPROPERTY(VisibleAnywhere)
    UResourceHealth*                ResourceHealth;
    UPROPERTY(BlueprintReadWrite)
    TWeakObjectPtr<ACharacterBase>  NearestCharacter;
    MapAllWeapons                   Weapons;
};
