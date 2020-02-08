#include "WeaponBase.h"
#include "TaskITM/TaskITM.h"

#include "TimerManager.h"
#include "Engine/World.h"

AWeaponBase::AWeaponBase()
    : Super()
{
    // Default settings
    bNeedAttack = false;
}

void AWeaponBase::Initialize_Implementation()
{}

void AWeaponBase::StartAttack()
{
    if (!IsAttack()) { StartTimer(0.f); }
    bNeedAttack     = true;
}

void AWeaponBase::EndAttack()
{
    bNeedAttack = false;
}

void AWeaponBase::OnAttack_Implementation(bool bEnable)
{}

bool AWeaponBase::IsAttack() const
{
    return IsValid(GetWorld()) ? GetWorld()->GetTimerManager().TimerExists(TimerHandler) : false;
}

void AWeaponBase::OnTimerWeapon()
{
    if (!bNeedAttack)   { OnAttack(false); EndTimer(); }
    else                { OnAttack(true); }
}

void AWeaponBase::PauseTimerWeapon(bool bEnable)
{
    UWorld* World = GetWorld();
    if (IsValid(World)) {
        FTimerManager& TimeManager = World->GetTimerManager();
        if (bEnable)    { TimeManager.PauseTimer(TimerHandler); }
        else            { TimeManager.UnPauseTimer(TimerHandler); }
    }
}
