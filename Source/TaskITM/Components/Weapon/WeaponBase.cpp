#include "WeaponBase.h"
#include "TaskITM/TaskITM.h"
#include "TaskITM/Characters/CharacterBase.h"

#include "TimerManager.h"
#include "Engine/World.h"

AWeaponBase::AWeaponBase()
    : Super()
{
    // Default settings
    bNeedAttack = false;
    bNeedReload = false;
}

void AWeaponBase::Initialize_Implementation()
{}

void AWeaponBase::PreDestroy_Implementation()
{}

void AWeaponBase::StartAttack()
{
    ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwner());
    if (IsValid(CharacterBase)) {
        ACharacterBase* NearestCharacter = CharacterBase->GetNearestCharacter();
        if (IsValid(NearestCharacter) && !NearestCharacter->bCallPreDestroy) {
            NearestCharacter->GetActorLocation();
            float SquareDistance = (NearestCharacter->GetActorLocation() - CharacterBase->GetActorLocation()).SizeSquared();
            if (SquareDistance <= Range * Range) {
                if (!IsAttack() && !bNeedReload) { StartTimer(0.f); }
                bNeedAttack = true;
                return;
            }
        }
    }
    EndAttack();
}

void AWeaponBase::EndAttack()
{
    bNeedAttack = false;
}

void AWeaponBase::OnAttack_Implementation(bool bEnable)
{}

bool AWeaponBase::IsAttack() const
{
    return IsValid(GetWorld()) ? GetWorld()->GetTimerManager().TimerExists(TimerHandler) && !bNeedReload : false;
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
