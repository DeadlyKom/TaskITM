#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class AWeaponBase;
class UChildActorComponent;

UCLASS()
class ACharacterBase : public ACharacter
{
    GENERATED_BODY()
    using MapAllWeapons = TMap<FName, TWeakObjectPtr<UChildActorComponent>>;
public:
                                    ACharacterBase();
    /// Functions
    UFUNCTION(BlueprintCallable)
    ACharacterBase*                 SearchNearestCharacter(TSubclassOf<ACharacterBase> Class);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                            SetNewWeapon(UChildActorComponent* ChildActorComponent, TSubclassOf<AWeaponBase> NewWeaponClass);
    virtual void                    SetNewWeapon_Implementation(UChildActorComponent* ChildActorComponent, TSubclassOf<AWeaponBase> NewWeaponClass);
    /// Variables
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                           VisionRange;

protected:
    UPROPERTY(BlueprintReadWrite)
    TWeakObjectPtr<ACharacterBase>  NearestCharacter;
    MapAllWeapons                   Weapons;
};
