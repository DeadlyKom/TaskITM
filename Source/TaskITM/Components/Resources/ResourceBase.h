#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceMinValueHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceMaxValueHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceSupplyHit, float, Value);

UCLASS(Abstract, Blueprintable)
class UResourceBase : public UActorComponent
{
    GENERATED_BODY()

public:
                            UResourceBase();
    
    UFUNCTION(BlueprintSetter)
    void                    Set(float Value);
    UFUNCTION(BlueprintGetter)
    float                   Get() const;
    UFUNCTION(BlueprintSetter)
    void                    SetMaxValue(float Value);
    UFUNCTION(BlueprintGetter)
    float                   GetMaxValue() const;
    UFUNCTION(BlueprintPure)
    float                   GetPercent() const;
    UFUNCTION(BlueprintPure)
    float                   GetInvPercent() const;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                    Supply(float Value);
    virtual void            Supply_Implementation(float Value);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                    Waste(float Value);
    virtual void            Waste_Implementation(float Value);

    FOnResourceMinValueHit& GetResourceMinValueHit();
    FOnResourceMaxValueHit& GetResourceMaxValueHit();
    FOnResourceSupplyHit&   GetResourceSupplyHit();

protected:
    // Events
    UPROPERTY(BlueprintAssignable)
    FOnResourceMinValueHit  OnResourceMinValueHit;
	UPROPERTY(BlueprintAssignable)
    FOnResourceMaxValueHit  OnResourceMaxValueHit;
    UPROPERTY(BlueprintAssignable)
    FOnResourceSupplyHit    OnResourceSupplyHit;
    // Variables
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "0"))
    float                   MinValue;
    UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMaxValue, BlueprintSetter = SetMaxValue, meta = (ClampMin = "1", ClampMax = "1000"))
    float                   MaxValue;
    UPROPERTY(BlueprintGetter = Get, BlueprintSetter = Set)
    float                   Resource;

private:
    FORCEINLINE void        CallingEvents();
};
