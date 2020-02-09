#include "WeaponLaser.h"
#include "TaskITM/TaskITM.h"
#include "TaskITM/Characters/CharacterBase.h"
#include "TaskITM/Components/Weapon/WeaponBase.h"
#include "TaskITM/Components/Resources/ResourceHealth.h"

#include "TimerManager.h"
#include "Engine/World.h"

AWeaponLaser::AWeaponLaser()
    : Super()
{
}

void AWeaponLaser::OnTimerWeapon()
{
    ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwner());
    if (IsValid(CharacterBase)) {
        ACharacterBase* NearestCharacter = CharacterBase->GetNearestCharacter();
        if (IsValid(NearestCharacter) && NearestCharacter->GetClass()->ImplementsInterface(UCharacterBaseClass::StaticClass())) {
            UResourceHealth* ResourceHealth = ICharacterBaseClass::Execute_GetResourceHealth(NearestCharacter);
            if (IsValid(ResourceHealth)) {
                ResourceHealth->Waste(DamagePerSecond);
            }
            if (NearestCharacter->bCallPreDestroy) {
                EndAttack();
            }
        }
    }

    EnergyElement -= WastEnergyPerSecond;

    if (EnergyElement <= 0) {
        EndTimer();
        OnAttack(false);
        bNeedReload = true;
        ReloadEnergy();
        return;
    }

    if (!bNeedAttack) {
        EndTimer();
        OnAttack(false);
        return;
    }
}

void AWeaponLaser::StartTimer(float InFirstDelay /*= -1.f*/)
{
    OnAttack(true);
}

void AWeaponLaser::EndTimer()
{
    UWorld* World = GetWorld();
    if (IsValid(World)) {
        FTimerManager& TimeManager = World->GetTimerManager();
        TimeManager.ClearTimer(TimerHandler);
    }
}

void AWeaponLaser::ActivateLaser_Implementation()
{
    UWorld* World = GetWorld();
    if (IsValid(World)) {
        FTimerManager& TimeManager = World->GetTimerManager();
        TimeManager.SetTimer(TimerHandler, this, &AWeaponBase::OnTimerWeapon, 1.f, true);
    }
}

void AWeaponLaser::ReloadEnergy_Implementation()
{
    Initialize();
    bNeedReload = false;
}

void AWeaponLaser::Initialize_Implementation()
{
    EnergyElement = EnergyElementSize;
}
