#include "PlayerSpectatorPawnBase.h"
#include "TaskITM/TaskITM.h"

#include "Engine/CollisionProfile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

namespace
{
    // Name components
    static const FName CollisionComponentName(TEXT("CollisionComponent"));
    static const FName SpringArmComponentName(TEXT("SpringArmComponent"));
    static const FName CameraComponentName(TEXT("CameraComponent"));
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
    bEnableCombinationToTurn            = false;
    // Collision
    CollisionComponent                  = CreateDefaultSubobject<USphereComponent>(CollisionComponentName);
    CollisionComponent->InitSphereRadius(35.0f);
    CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    CollisionComponent->SetVisibility(false);
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

void APlayerSpectatorPawnBase::EnableCombinationToTurns()
{
    bEnableCombinationToTurn = true;
}

void APlayerSpectatorPawnBase::DisableCombinationToTurns()
{
    bEnableCombinationToTurn = false;
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
    if (!FMath::IsNearlyZero(Value)) {
        FVector Location = GetActorLocation();
        Location.Z = FMath::Clamp(Location.Z + Value, DefaultHeight, DefaultHeight + MaxLiftHeight);
        SetActorLocation(Location);
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
