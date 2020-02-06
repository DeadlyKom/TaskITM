#include "PlayerSpectatorPawnBase.h"
#include "TaskITM/TaskITM.h"
#include "PlayerCharacter.h"
#include "PlayerControllerBase.h"

#include "Engine/CollisionProfile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

namespace
{
    // Name components
    static const FName CollisionComponentName(TEXT("CollisionComponent"));
    static const FName SpringArmComponentName(TEXT("SpringArmComponent"));
    static const FName CameraComponentName(TEXT("CameraComponent"));
    static const FName CursorComponentName(TEXT("CursorComponent"));
    static const FName MovementComponentName(TEXT("MovementComponent"));
    // Moving forward and backward on the X-axis (surge)
    static const FName MovingForwardBackwardName(TEXT("MovingForwardBackward"));
    // Moving left and right on the Y-axis (sway)
    static const FName MovingLeftRightName(TEXT("MovingLeftRight"));
    // Moving up and down on the Z-axis (heave)
    static const FName MovingUpDownName(TEXT("MovingUpDown"));
    // Tilting forward and backward on the Y-axis (pitch)
    static const FName TiltingForwardBackwardName(TEXT("TiltingForwardBackward"));
    // Turning left and right on the Z-axis (Yaw)
    static const FName TurningLeftRightName(TEXT("TurningLeftRight"));
    // 
    static const FName CharacterSelectionButtonName(TEXT("CharacterSelectionButton"));
    static const FName CharacterActionButtonName(TEXT("CharacterActionButton"));
    // 
    static const FName CombinationForTurnsName(TEXT("CombinationForTurns"));
}

APlayerSpectatorPawnBase::APlayerSpectatorPawnBase()
    : Super()
{
    /// Setting default 
    SetCanBeDamaged(false);
    bCollideWhenPlacing                 = false;
    SpawnCollisionHandlingMethod        = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    BaseTurnRate                        = 45.f;
    BaseLookUpRate                      = 45.f;
    MaxLiftHeight                       = 500.f;
    MinClickDistance                    = 120.f;
    bEnableCombinationToTurn            = false;
    // Collision
    CollisionComponent                  = CreateDefaultSubobject<USphereComponent>(CollisionComponentName);
    CollisionComponent->InitSphereRadius(35.0f);
    CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    CollisionComponent->SetVisibility(true);
    RootComponent                       = CollisionComponent;
    // Spring
    SpringArm                           = CreateDefaultSubobject<USpringArmComponent>(SpringArmComponentName);
    SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    SpringArm->TargetArmLength          = 0.f;
    SpringArm->SocketOffset             = FVector(-200.f, 0.f, 75.f);
    SpringArm->TargetOffset             = FVector::ZeroVector;
    SpringArm->bDoCollisionTest         = false;
    SpringArm->bEnableCameraLag         = false;
    SpringArm->bEnableCameraRotationLag = false;
    // Camera
    Camera                              = CreateDefaultSubobject<UCameraComponent>(CameraComponentName);
    Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation     = true;
    // Deacl
    Cursor                              = CreateDefaultSubobject<UDecalComponent>(CursorComponentName);
    Cursor->DecalSize                   = FVector(16.f, 32.f, 32.f);
    Cursor->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
    Cursor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    // Movement
    MovementComponent                   = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(MovementComponentName);
    MovementComponent->UpdatedComponent = CollisionComponent;
}

void APlayerSpectatorPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);
    //
    PlayerInputComponent->BindAction(CombinationForTurnsName, EInputEvent::IE_Pressed, this, &APlayerSpectatorPawnBase::EnableCombinationToTurns);
    PlayerInputComponent->BindAction(CombinationForTurnsName, EInputEvent::IE_Released, this, &APlayerSpectatorPawnBase::DisableCombinationToTurns);
    //
    PlayerInputComponent->BindAction(CharacterSelectionButtonName, EInputEvent::IE_Pressed, this, &APlayerSpectatorPawnBase::CharacterSelectionButtonPressed);
    PlayerInputComponent->BindAction(CharacterSelectionButtonName, EInputEvent::IE_Released, this, &APlayerSpectatorPawnBase::CharacterSelectionButtonReleased);
    //
    PlayerInputComponent->BindAction(CharacterActionButtonName, EInputEvent::IE_Pressed, this, &APlayerSpectatorPawnBase::CharacterActionButtonPressed);
    PlayerInputComponent->BindAction(CharacterActionButtonName, EInputEvent::IE_Released, this, &APlayerSpectatorPawnBase::CharacterActionButtonReleased);
    // Binds translational envelopes
    PlayerInputComponent->BindAxis(MovingForwardBackwardName,   this, &APlayerSpectatorPawnBase::MovingForwardBackward);
    PlayerInputComponent->BindAxis(MovingLeftRightName,         this, &APlayerSpectatorPawnBase::MovingLeftRight);
    PlayerInputComponent->BindAxis(MovingUpDownName,            this, &APlayerSpectatorPawnBase::MovingUpDown);
    // Binds rotational envelopes
    PlayerInputComponent->BindAxis(TiltingForwardBackwardName,  this, &APlayerSpectatorPawnBase::TiltingForwardBackward);
    PlayerInputComponent->BindAxis(TurningLeftRightName,        this, &APlayerSpectatorPawnBase::TurningLeftRight);
}

void APlayerSpectatorPawnBase::BeginPlay()
{
    Super::BeginPlay();
    // Save start height
    DefaultHeight = GetActorLocation().Z;
}

void APlayerSpectatorPawnBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APlayerControllerBase* PlayerSpectatorController = Cast<APlayerControllerBase>(Controller);
    if (IsValid(PlayerSpectatorController)) {
        FHitResult Hit;
        PlayerSpectatorController->GetHitResultUnderCursorByChannel((ETraceTypeQuery)ECollisionChannel::ECC_Visibility, true, Hit);
        Cursor->SetVisibility(Hit.bBlockingHit && !bEnableCombinationToTurn);
        if (Hit.bBlockingHit && !bEnableCombinationToTurn) {
            Cursor->SetWorldLocationAndRotation(Hit.Location, Hit.ImpactNormal.ToOrientationQuat());
        }
    }
}

void APlayerSpectatorPawnBase::EnableCombinationToTurns()
{
    bEnableCombinationToTurn = true;
}

void APlayerSpectatorPawnBase::DisableCombinationToTurns()
{
    bEnableCombinationToTurn = false;
}

void APlayerSpectatorPawnBase::CharacterSelectionButtonPressed()
{
    APlayerControllerBase* PlayerSpectatorController = Cast<APlayerControllerBase>(Controller);
    if (IsValid(PlayerSpectatorController)) {
        FHitResult Hit;
        PlayerSpectatorController->GetHitResultUnderCursorByChannel((ETraceTypeQuery)ECollisionChannel::ECC_Visibility, true, Hit);
        if (Hit.bBlockingHit) {
            APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(Hit.Actor);
            SetCharacterSelection(PlayerCharacter);
        }
    }
}

void APlayerSpectatorPawnBase::CharacterSelectionButtonReleased()
{
}

void APlayerSpectatorPawnBase::CharacterActionButtonPressed()
{
    if (CharacterSelection.IsValid()) {
        APlayerControllerBase* PlayerSpectatorController = Cast<APlayerControllerBase>(Controller);
        if (IsValid(PlayerSpectatorController)) {
            FHitResult Hit;
            PlayerSpectatorController->GetHitResultUnderCursorByChannel((ETraceTypeQuery)ECollisionChannel::ECC_Visibility, true, Hit);
            if (Hit.bBlockingHit) {
                if ((CharacterSelection->GetActorLocation() - Hit.Location).SizeSquared() > MinClickDistance * MinClickDistance) {
                    UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterSelection->GetController(), Hit.Location);
                }
            }
        }
    }
}

void APlayerSpectatorPawnBase::CharacterActionButtonReleased()
{

}

void APlayerSpectatorPawnBase::MovingForwardBackward(float Value)
{
    if (!FMath::IsNearlyZero(Value) && IsValid(Controller)) {
        FRotator ControlSpaceRot = Controller->GetControlRotation();
        FVector ForwardVector = FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X);
        ForwardVector.Z = 0;
        AddMovementInput(ForwardVector.GetSafeNormal(), Value);
    }
}

void APlayerSpectatorPawnBase::MovingLeftRight(float Value)
{
    if (!FMath::IsNearlyZero(Value) && IsValid(Controller)) {
        FRotator ControlSpaceRot = Controller->GetControlRotation();
        FVector RightVector = FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y);
        AddMovementInput(RightVector, Value);
    }
}

void APlayerSpectatorPawnBase::MovingUpDown(float Value)
{
    if (!FMath::IsNearlyZero(Value) && IsValid(Controller)) {
        FVector SpectatorLocation   = GetActorLocation();
        FRotator ControlSpaceRot    = Controller->GetControlRotation();
        FVector ForwardVector       = FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X);
        float VerticalOffset        = FVector::DotProduct(ForwardVector * Value, FVector::UpVector);
        // Clamp bottom plane
        if (VerticalOffset > 0) {
            float DistanceToPlane = FVector::DotProduct(SpectatorLocation - FVector(SpectatorLocation.X, SpectatorLocation.Y, DefaultHeight), FVector::UpVector);
            if (DistanceToPlane - VerticalOffset > 0.f) {
                SetActorLocation(SpectatorLocation - ForwardVector * Value);
            }
        }
        // Clamp top plane
        else {
            float DistanceToPlane = FVector::DotProduct(SpectatorLocation - FVector(SpectatorLocation.X, SpectatorLocation.Y, DefaultHeight + MaxLiftHeight), FVector::DownVector);
            if (DistanceToPlane - VerticalOffset > 0.f) {
                SetActorLocation(SpectatorLocation - ForwardVector * Value);
            }
        }
    }
}

void APlayerSpectatorPawnBase::TiltingForwardBackward(float Value)
{
    if (bEnableCombinationToTurn && !FMath::IsNearlyZero(Value)) {
        // Calculate delta for this frame from the rate information
        AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
    }
}

void APlayerSpectatorPawnBase::TurningLeftRight(float Value)
{
    if (bEnableCombinationToTurn && !FMath::IsNearlyZero(Value)) {
        // Calculate delta for this frame from the rate information
        AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
    }
}

void APlayerSpectatorPawnBase::SetCharacterSelection(APlayerCharacterBase* NewCharacterSelection)
{
    if (CharacterSelection.IsValid()) {
        CharacterSelection->SetCharacterSelection(false);
        CharacterSelection.Reset();
    }
    if (IsValid(NewCharacterSelection)) {
        NewCharacterSelection->SetCharacterSelection(true);
        CharacterSelection = NewCharacterSelection;
    }
}
