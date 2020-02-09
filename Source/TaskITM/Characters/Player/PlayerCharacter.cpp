#include "PlayerCharacter.h"
#include "TaskITM/TaskITM.h"
#include "TaskITM/Characters/AI/AICharacter.h"

APlayerCharacterBase::APlayerCharacterBase()
    : Super()
{}

void APlayerCharacterBase::SetCharacterSelection_Implementation(bool bEnable)
{}

void APlayerCharacterBase::TurnWeaponToNearTarget_Implementation(AActor* NearTarget)
{}

void APlayerCharacterBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SearchNearestCharacter(AAICharacterBase::StaticClass());
    TurnWeaponToNearTarget(NearestCharacter.Get());
}
