#include "ResourceHealth.h"
#include "TaskITM/TaskITM.h"

UResourceHealth::UResourceHealth()
    : Super()
{
    // Default setting
    DefaultHealth = MaxValue;
}

void UResourceHealth::BeginPlay()
{
    Super::BeginPlay();
    Set(DefaultHealth);
}
