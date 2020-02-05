#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerSpectatorPawnBase.generated.h"

class UInputComponent;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UPawnMovementComponent;

UCLASS()
class APlayerSpectatorPawnBase : public APawn
{
    GENERATED_BODY()
public:

	                                APlayerSpectatorPawnBase();
    /// Override
    virtual void                    SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    /// Variables
    // Base turn rate, in deg/sec. Other scaling may affect final turn rate
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float                           BaseTurnRate;
    // Base lookup rate, in deg/sec. Other scaling may affect final lookup rate
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float                           BaseLookUpRate;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float                           MaxLiftHeight;

protected:
    /// Override
    virtual void                    BeginPlay() override;

    /// Functions

    //
    void                            EnableCombinationToTurns();
    void                            DisableCombinationToTurns();
    // Translational envelopes
    void                            MovingForwardBackward(float Value);
    void                            MovingLeftRight(float Value);
    void                            MovingUpDown(float Value);
    // Rotational envelopes
    void                            TiltingForwardBackward(float Value);
    void                            TurningLeftRight(float Value);

private:
    /// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent*               CollisionComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent*            SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent*               Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent*         MovementComponent;
    //
    float                           DefaultHeight;
    uint8                           bEnableCombinationToTurn : 1;
};