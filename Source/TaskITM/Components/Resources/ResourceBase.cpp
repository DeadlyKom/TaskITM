#include "ResourceBase.h"
#include "TaskITM/TaskITM.h"

UResourceBase::UResourceBase()
    : Super()
{
    // Default setting
    MinValue = 0.f;
    MaxValue = 100.f;
    Resource = MaxValue;
}

void UResourceBase::Set(float Value)
{
    Resource = FMath::Clamp<float>(Value, MinValue, MaxValue);
}

float UResourceBase::Get() const
{
    return Resource;
}

void UResourceBase::SetMaxValue(float Value)
{
    MaxValue = Value;
}

float UResourceBase::GetMaxValue() const
{
    return MaxValue;
}

float UResourceBase::GetPercent() const
{
    return Resource / FMath::Abs(MaxValue - MinValue);
}

float UResourceBase::GetInvPercent() const
{
    return 1.f - Resource / FMath::Abs(MaxValue - MinValue);
}

FORCEINLINE void UResourceBase::CallingEvents()
{
    if      (Resource == MinValue && OnResourceMinValueHit.IsBound())   { OnResourceMinValueHit.Broadcast(); }
    else if (Resource == MaxValue && OnResourceMaxValueHit.IsBound())   { OnResourceMaxValueHit.Broadcast(); }
}

void UResourceBase::Supply_Implementation(float Value)
{
    Set(Resource + FMath::Abs(Value));
    OnResourceSupplyHit.Broadcast(Value);
    CallingEvents();
}

void UResourceBase::Waste_Implementation(float Value)
{
    Set(Resource - FMath::Abs(Value));
    CallingEvents();
}

FOnResourceMinValueHit& UResourceBase::GetResourceMinValueHit()
{
    return OnResourceMinValueHit;
}

FOnResourceMaxValueHit& UResourceBase::GetResourceMaxValueHit()
{
    return OnResourceMaxValueHit;
}

FOnResourceSupplyHit& UResourceBase::GetResourceSupplyHit()
{
    return OnResourceSupplyHit;
}
