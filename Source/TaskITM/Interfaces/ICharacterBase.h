#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacterBase.generated.h"

UINTERFACE(BlueprintType)
class UCharacterBaseClass : public UInterface
{
    GENERATED_UINTERFACE_BODY()
};

class ICharacterBaseClass
{
    GENERATED_IINTERFACE_BODY()
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    class UResourceHealth*          GetResourceHealth() const;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    class AWeaponBase*              GetWeapon(FName NameWeapon) const;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    TArray<class AWeaponBase*>      GetWeapons() const;
};
