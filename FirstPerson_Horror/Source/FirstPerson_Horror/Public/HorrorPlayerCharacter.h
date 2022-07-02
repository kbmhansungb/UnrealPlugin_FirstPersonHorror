// Bummoo Kim, 2022, Free license

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DelegateEvent.h"
#include <Kismet/KismetMathLibrary.h>
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

#pragma region Movement

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadWrite)
	bool AllowJump = true;

	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadWrite)
	bool AllowMovement = true;

	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadWrite)
	bool AllowControllerInput = true;

public:
	UFUNCTION(Category = "Movement", BlueprintCallable)
	void SetAllowJump(bool NewState);

	UFUNCTION(Category = "Movement", BlueprintCallable)
	void SetAllowMovement(bool NewState);

	UFUNCTION(Category = "Movement", BlueprintCallable)
	void SetAllowControllerInput(bool NewState);

protected:
	void Jump();
	void StopJumping();

	void MoveForward(float Val);
	void MoveRight(float Val);

	void AddControllerYawInput(float Val);
	void AddControllerPitchInput(float Val);

	void AddControllerRotator(const FRotator& Rotator);

public:
	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadWrite)
	bool IsFootHitComplex = true;

public:
	FFootStrikeDelegate FootStrikeDelegate;
	
	UFUNCTION(BlueprintCallable)
	void CallFootStrike(FName SocketName, float Speed, ECollisionChannel TraceChannel, const FVector& Offset);

protected:
	virtual void TraceFoot(ECollisionChannel TraceChannel, const FVector& Start, const FVector& End, FFootHitEvent& FootHitEvent);

	UFUNCTION(Category = "Movement", BlueprintImplementableEvent)
	void PlayFootSound(const FFootHitEvent& FootHitEvent);

	UFUNCTION(Category = "Movement", BlueprintImplementableEvent)
	void ShakeCameraFromFoot(const FFootHitEvent& FootHitEvent);
	
#pragma endregion

#pragma region Camera

protected:
	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	bool IsInDesireCameraRotation;

	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	FRotator DesireCameraRotator;

	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	float CameraLookAtSpeed;

	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly)
	float CameraLookAtPrograss;

public:
	UFUNCTION(Category = "Camera", BlueprintCallable)
	void RotateCameraToLookAt(const FVector& NewLookAt, float Duration = 0.2f);

protected:
	void StartRotateCamera();
	void EndRotateCamera();
	virtual void UpdateCameraRotationToDesire(float DeletaTime);

protected:
	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	bool IsInDOF;

	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	bool IsDOFIn;
	
	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	float DOFSpeed;

	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	float DOFDistanceVector;
	
	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	float DOFPrograss;

	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	float DOFWeight;

	UPROPERTY(Category = "Camera", EditAnywhere, BlueprintReadWrite)
	float SetCircleDOFFromFocus = 3000.f;

	void StartDOF();
	void EndDOF();

public:
	UFUNCTION(Category = "Camera", BlueprintCallable)
	void DOFIn(const FVector& Location, float Duration = 0.2f);

	UFUNCTION(Category = "Camera", BlueprintCallable)
	void DOFOut(float Duration = 0.2f);

protected:
	void UpdateFocusEvent(float DeltaTime);

public:
	UFUNCTION(Category = "Camera", BlueprintCallable)
	void FocusIn(const FVector& Location, float Duration = 0.2f);

	UFUNCTION(Category = "Camera", BlueprintCallable)
	void FocusOut(float Duration = 0.2f);

#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MovementTag;

	void UpdateMovementTag();

private:
	void BaseChange() override;
};
