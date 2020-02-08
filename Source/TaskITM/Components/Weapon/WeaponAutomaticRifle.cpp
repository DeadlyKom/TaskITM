#include "WeaponAutomaticRifle.h"
#include "TaskITM/TaskITM.h"

#include "TimerManager.h"
#include "Engine/World.h"

AWeaponAutomaticRifle::AWeaponAutomaticRifle()
    : Super()
{
}

void AWeaponAutomaticRifle::StartTimer(float InFirstDelay /*= -1.f*/)
{
    UWorld* World = GetWorld();
    if (IsValid(World)) {
        FTimerManager& TimeManager = World->GetTimerManager();
        TimeManager.SetTimer(TimerHandler, this, &AWeaponBase::OnTimerWeapon, 60.f / RoundsPerMinute, true, InFirstDelay);
    }
}

void AWeaponAutomaticRifle::EndTimer()
{
    UWorld* World = GetWorld();
    if (IsValid(World)) {
        FTimerManager& TimeManager = World->GetTimerManager();
        TimeManager.ClearTimer(TimerHandler);
    }
}
