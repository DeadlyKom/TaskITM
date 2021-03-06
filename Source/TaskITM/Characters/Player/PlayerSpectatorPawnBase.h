#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerSpectatorPawnBase.generated.h"

class UInputComponent;
class UDecalComponent;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UPawnMovementComponent;

class APlayerCharacterBase;

UCLASS()
class APlayerSpectatorPawnBase : public APawn
{
    GENERATED_BODY()

    using PlayerCharacter = APlayerCharacterBase;
public:

	                                APlayerSpectatorPawnBase();
    /// Functions
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void                            DisplayTargetOfMovement(const FVector& LocationTarget);
    virtual void                    DisplayTargetOfMovement_Implementation(const FVector& LocationTarget);
    /// Variables
    // Base turn rate, in deg/sec. Other scaling may affect final turn rate
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                           BaseTurnRate;
    // Base lookup rate, in deg/sec. Other scaling may affect final lookup rate
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                           BaseLookUpRate;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                           MaxLiftHeight;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float                           MinClickDistance;

protected:
    /// Override
    virtual void                    BeginPlay() override;
    virtual void                    Tick(float DeltaSeconds) override;
    virtual void                    SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    /// Functions

    //
    void                            EnableCombinationToTurns();
    void                            DisableCombinationToTurns();
    //
    void                            CharacterSelectionButtonPressed();
    void                            CharacterSelectionButtonReleased();
    //
    void                            CharacterActionButtonPressed();
    void                            CharacterActionButtonReleased();
    // Translational envelopes
    void                            MovingForwardBackward(float Value);
    void                            MovingLeftRight(float Value);
    void                            MovingUpDown(float Value);
    // Rotational envelopes
    void                            TiltingForwardBackward(float Value);
    void                            TurningLeftRight(float Value);

private:
    /// Functions
    void                            SetCharacterSelection(APlayerCharacterBase* NewCharacterSelection);
    /// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent*               CollisionComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent*            SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent*               Camera;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UDecalComponent*                Cursor;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent*         MovementComponent;
    //
    float                           DefaultHeight;
    uint8                           bEnableCombinationToTurn : 1;
    TWeakObjectPtr<PlayerCharacter> CharacterSelection;
};
