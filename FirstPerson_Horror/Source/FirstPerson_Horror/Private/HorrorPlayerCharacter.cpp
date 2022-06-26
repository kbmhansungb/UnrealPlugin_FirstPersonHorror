// Bummoo Kim, 2022, Free license


#include "HorrorPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AHorrorPlayerCharacter::AHorrorPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultCamera = CreateDefaultSubobject<UCameraComponent>(FName("DefaultCamera"));
	DefaultCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DefaultCamera->bUsePawnControlRotation = true;
}


// Called when the game starts or when spawned
void AHorrorPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	FootStrikeDelegate.AddUObject(this, &AHorrorPlayerCharacter::PlayFootSound);

	if (Cast<APlayerController>(GetController()))
	{
		FootStrikeDelegate.AddUObject(this, &AHorrorPlayerCharacter::ShakeCameraFromFoot);
	}
}

// Called every frame
void AHorrorPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHorrorPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHorrorPlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AHorrorPlayerCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHorrorPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHorrorPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AHorrorPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AHorrorPlayerCharacter::AddControllerPitchInput);
}

void AHorrorPlayerCharacter::SetAllowJump(bool NewState)
{
	AllowJump = NewState;
}

void AHorrorPlayerCharacter::SetAllowMovement(bool NewState)
{
	AllowMovement = NewState;
}

void AHorrorPlayerCharacter::SetAllowControllerInput(bool NewState)
{
	AllowControllerInput = NewState;
}

void AHorrorPlayerCharacter::Jump()
{
	if (AllowJump == false)
		return;

	ACharacter::Jump();
}

void AHorrorPlayerCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void AHorrorPlayerCharacter::MoveForward(float Val)
{
	if (AllowMovement == false)
		return;

	AddMovementInput(GetCapsuleComponent()->GetForwardVector(), Val);
}

void AHorrorPlayerCharacter::MoveRight(float Val)
{
	if (AllowMovement == false)
		return;

	AddMovementInput(GetCapsuleComponent()->GetRightVector(), Val);
}

void AHorrorPlayerCharacter::AddControllerYawInput(float Val)
{
	if (AllowControllerInput == false)
		return;

	APawn::AddControllerYawInput(Val);
}

void AHorrorPlayerCharacter::AddControllerPitchInput(float Val)
{
	if (AllowControllerInput == false)
		return;

	APawn::AddControllerPitchInput(Val);
}

void AHorrorPlayerCharacter::CallFootStrike(FName SocketName, float FootVelocityLength, ECollisionChannel TraceChannel)
{
	FFootHitEvent FootHitEvent;
	FootHitEvent.FootVelocityLength = FootVelocityLength;
	
	if (GetMesh()->SkeletalMesh)
	{
		FHitResult HitResult;
		FVector Start = GetMesh()->GetSocketLocation(SocketName);
		const FVector Offset = FVector(0.0f, 0.0f, -15.0f);

		FootHitEvent.IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, Start - Offset, TraceChannel);
		FootHitEvent.HitLocation = HitResult.Location;
		FootHitEvent.HitPhysicsMaterial = HitResult.PhysMaterial.Get();
	}

	if (FootStrikeDelegate.IsBound())
		FootStrikeDelegate.Broadcast(FootHitEvent);
}

void AHorrorPlayerCharacter::UpdateMovementTag()
{
	UPrimitiveComponent* MovementBase = GetMovementBase();
	if (MovementBase != nullptr)
	{
		int32 Index = MovementBase->ComponentTags.Find(FName("Movement"));

#if WITH_EDITOR
		if (MovementBase->ComponentTags.IsValidIndex(Index + 1) == false)
		{
			MovementTag = FName();
			UE_LOG(LogTemp, Error, TEXT("InValid Index"));
			return;
		}
#endif
		MovementTag = MovementBase->ComponentTags[Index + 1];
	}
	else
	{
		MovementTag = FName();
	}
}

void AHorrorPlayerCharacter::BaseChange()
{
	Super::BaseChange();

	UpdateMovementTag();
}

