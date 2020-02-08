#include "WeaponLaser.h"
#include "TaskITM/TaskITM.h"

#include "TimerManager.h"
#include "Engine/World.h"

AWeaponLaser::AWeaponLaser()
    : Super()
{
}

void AWeaponLaser::OnTimerWeapon()
{
    if (!bNeedAttack)   { OnAttack(false); EndTimer(); }
    else                { OnAttack(true); }
}

void AWeaponLaser::StartTimer(float InFirstDelay /*= -1.f*/)
{
    OnTimerWeapon();
}

void AWeaponLaser::EndTimer()
{
}
