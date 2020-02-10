#include "CharacterBase.h"
#include "TaskITM/TaskITM.h"
#include "TaskITM/Components/Weapon/WeaponBase.h"
#include "TaskITM/Components/Resources/ResourceHealth.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/Public/EngineUtils.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

namespace
{
    static const FName ResourceHealthComponentName(TEXT("ResourceHealth"));
}

ACharacterBase::ACharacterBase()
    : Super()
{
    // Default setting
    bCallPreDestroy = false;
    ResourceHealth  = CreateDefaultSubobject<UResourceHealth>(ResourceHealthComponentName);
}

UResourceHealth* ACharacterBase::GetResourceHealth_Implementation() const
{
    return ResourceHealth;
}

AWeaponBase* ACharacterBase::GetWeapon_Implementation(FName NameWeapon) const
{
    auto* FoundWeapon = Weapons.Find(NameWeapon);
    if (FoundWeapon != nullptr && FoundWeapon->IsValid()) {
        return Cast<AWeaponBase>(FoundWeapon->Get()->GetChildActor());
    }
    return nullptr;
}

TArray<AWeaponBase*> ACharacterBase::GetWeapons_Implementation() const
{
    TArray<AWeaponBase*> ResultWeapons;
    for (auto& Weapon : Weapons) {
        if (Weapon.Value.IsValid()) {
            ResultWeapons.Add(Cast<AWeaponBase>(Weapon.Value->GetChildActor()));
        }
    }
    return ResultWeapons;
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
            if (IsValid(*It) && !It->bCallPreDestroy) {
                FVector LocationCharacter   = It->GetActorLocation();
                float SquareDistance        = (Location - LocationCharacter).SizeSquared();
                if (SquareDistance <= SquareDistanceMin /* && It->WasRecentlyRendered(0.f)*/) {
                    FHitResult Hit;
                    FCollisionQueryParams QueryParams;
                    QueryParams.AddIgnoredActor(this);
                    for (const auto& Weapon : Weapons) { if (Weapon.Value.IsValid()) { QueryParams.AddIgnoredActor(Weapon.Value->GetChildActor()); } }
                    QueryParams.bTraceComplex           = true;
                    QueryParams.bReturnPhysicalMaterial = false;
                    QueryParams.bReturnFaceIndex        = false;
                    if (World->LineTraceSingleByChannel(Hit, Location, LocationCharacter, ECollisionChannel::ECC_Visibility, QueryParams)) {
                        //DrawDebugLine(World, Location, LocationCharacter, FColor::Red, false, -1.f, 0, 3.f);
                        if (Hit.Actor->IsA(Class)) {
                            SquareDistanceMin = SquareDistance;
                            NearestCharacter        = *It;
                        }
                        AActor* ActorOwner = Hit.Actor->GetOwner();
                        if (IsValid(ActorOwner) && ActorOwner->IsA(Class)) {
                            SquareDistanceMin = SquareDistance;
                            NearestCharacter = *It;
                        }
                    }
                }
            }
        }
    }
    return NearestCharacter.Get();
}

ACharacterBase* ACharacterBase::GetNearestCharacter() const
{
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

void ACharacterBase::PreDestroy_Implementation()
{
    Destroy();
}

void ACharacterBase::BeginPlay()
{
    Super::BeginPlay();
    OnTakeAnyDamage.AddDynamic(this, &ACharacterBase::OnTakeAnyDamageEvent);
    ResourceHealth->GetResourceMinValueHit().AddDynamic(this, &ACharacterBase::OnResourceMinValueHitEvent);
}

void ACharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    OnTakeAnyDamage.RemoveDynamic(this, &ACharacterBase::OnTakeAnyDamageEvent);
    ResourceHealth->GetResourceMinValueHit().RemoveDynamic(this, &ACharacterBase::OnResourceMinValueHitEvent);
}

void ACharacterBase::OnResourceMinValueHitEvent_Implementation()
{
    if (!bCallPreDestroy) {
        bCallPreDestroy = true;
        PreDestroy();
    }
}

void ACharacterBase::OnTakeAnyDamageEvent_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    ResourceHealth->Waste(Damage);
}
