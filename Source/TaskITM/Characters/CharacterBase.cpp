#include "CharacterBase.h"
#include "TaskITM/TaskITM.h"
#include "TaskITM/Components/Weapon/WeaponBase.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/Public/EngineUtils.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterBase::ACharacterBase()
    : Super()
{
}

//#include "Runtime/Engine/Public/DrawDebugHelpers.h"

ACharacterBase* ACharacterBase::SearchNearestCharacter(TSubclassOf<ACharacterBase> Class)
{
    NearestCharacter.Reset();
    UWorld* World = GetWorld();
    if (IsValid(World)) {
        float SquareDistanceMin = VisionRange * VisionRange;
        FVector Location        = GetActorLocation();
        for (TActorIterator<ACharacterBase> It(World, Class); It; ++It) {
            FVector LocationCharacter   = It->GetActorLocation();
            float SquareDistance        = (Location - LocationCharacter).SizeSquared();
            if (SquareDistance <= SquareDistanceMin/* && It->WasRecentlyRendered(0.f)*/) {
                FHitResult Hit;
                FCollisionQueryParams QueryParams;
                QueryParams.AddIgnoredActor(this);
                QueryParams.bTraceComplex           = true;
                QueryParams.bReturnPhysicalMaterial = false;
                QueryParams.bReturnFaceIndex        = false;
                if (World->LineTraceSingleByChannel(Hit, Location, LocationCharacter, ECollisionChannel::ECC_Visibility, QueryParams)) {
                    //DrawDebugLine(World, Location, LocationCharacter, FColor::Red, false, -1.f, 0, 3.f);
                    SquareDistanceMin = SquareDistance;
                    NearestCharacter        = *It;
                }
            }
        }
    }
    return NearestCharacter.Get();
}

void ACharacterBase::SetNewWeapon_Implementation(UChildActorComponent* ChildActorComponent, TSubclassOf<AWeaponBase> NewWeaponClass)
{
    if (IsValid(ChildActorComponent)) {
        ChildActorComponent->SetChildActorClass(NewWeaponClass);
        AWeaponBase* Weapon = Cast<AWeaponBase>(ChildActorComponent->GetChildActor());
        if (IsValid(Weapon)) {
            Weapons.FindOrAdd(Weapon->WeaponID, MakeWeakObjectPtr(ChildActorComponent));
            Weapon->SetOwner(this);
            Weapon->Initialize();
        }
    }
}
