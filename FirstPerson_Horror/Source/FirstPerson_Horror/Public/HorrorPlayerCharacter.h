// Bummoo Kim, 2022, Free license

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DelegateEvent.h"
#include "HorrorPlayerCharacter.generated.h"

class UCameraComponent;

UCLASS(BlueprintType)
class FIRSTPERSON_HORROR_API AHorrorPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHorrorPlayerCharacter();

protected:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* DefaultCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(Category = "Pawn|Input", EditAnywhere, BlueprintReadWrite)
	bool AllowJump = true;

	UPROPERTY(Category = "Pawn|Input", EditAnywhere, BlueprintReadWrite)
	bool AllowMovement = true;

	UPROPERTY(Category = "Pawn|Input", EditAnywhere, BlueprintReadWrite)
	bool AllowControllerInput = true;

public:
	UFUNCTION(Category = "Pawn|Input", BlueprintCallable)
	void SetAllowJump(bool NewState);

	UFUNCTION(Category = "Pawn|Input", BlueprintCallable)
	void SetAllowMovement(bool NewState);

	UFUNCTION(Category = "Pawn|Input", BlueprintCallable)
	void SetAllowControllerInput(bool NewState);

protected:
	void Jump();
	void StopJumping();

	void MoveForward(float Val);
	void MoveRight(float Val);

	void AddControllerYawInput(float Val);
	void AddControllerPitchInput(float Val);

#pragma region FootStrike

public:
	FFootStrikeDelegate FootStrikeDelegate;

	/**
	 * Passed from the Anim instance to the Notify.
	 */
	UFUNCTION(BlueprintCallable)
	void CallFootStrike(FName SocketName, float FootVelocityLength, ECollisionChannel TraceChannel);

protected:
	UFUNCTION(Category = "FootStrike", BlueprintImplementableEvent)
	void PlayFootSound(const FFootHitEvent& FootHitEvent);

	UFUNCTION(Category = "FootStrike", BlueprintImplementableEvent)
	void ShakeCameraFromFoot(const FFootHitEvent& FootHitEvent);
	
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MovementTag;

	void UpdateMovementTag();

private:
	void BaseChange() override;
};
