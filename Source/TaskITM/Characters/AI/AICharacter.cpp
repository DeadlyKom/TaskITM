#include "AICharacter.h"
#include "TaskITM/TaskITM.h"
#include "TaskITM/Characters/Player/PlayerCharacter.h"

AAICharacterBase::AAICharacterBase()
    : Super()
{}

void AAICharacterBase::TurnWeaponToNearTarget_Implementation(AActor* NearTarget)
{}

void AAICharacterBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SearchNearestCharacter(APlayerCharacterBase::StaticClass());
    TurnWeaponToNearTarget(NearestCharacter.Get());
}
