// Bummoo Kim, 2022, Free license


#include "HorrorPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/GameEngine.h"
#include "DrawDebugHelpers.h"

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

void AHorrorPlayerCharacter::CallFootStrike(FName SocketName, float FootVelocityLength, ECollisionChannel TraceChannel, const FVector& Offset)
{
#if !NO_CVARS
	static const auto HPC_DebugFootStrike = IConsoleManager::Get().FindConsoleVariable(TEXT("HPC.DebugFootStrike"));
#endif

	FFootHitEvent FootHitEvent;
	FootHitEvent.FootVelocityLength = FootVelocityLength;

	if (GetMesh()->SkeletalMesh)
	{
		FHitResult HitResult;
		const FVector Start = GetMesh()->GetSocketLocation(SocketName) + Offset;
		const FVector End = GetMesh()->GetSocketLocation(SocketName) - Offset;

		TArray<AActor*> TraceIgnore;
		if (GetMesh()->GetOwner())
		{
			TraceIgnore.Add(GetMesh()->GetOwner());
		}

		FootHitEvent.IsHit = UKismetSystemLibrary::LineTraceSingle(
			this, 
			Start, 
			End, 
			UEngineTypes::ConvertToTraceType(TraceChannel), 
			IsFootHitComplex,
			TraceIgnore, 
#if ENABLE_DRAW_DEBUG && !NO_CVARS
			HPC_DebugFootStrike->GetBool() ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
#else
			EDrawDebugTrace::None,
#endif
			HitResult, 
			false
		);
		FootHitEvent.HitLocation = HitResult.Location;
		FootHitEvent.HitPhysicsMaterial = HitResult.PhysMaterial.Get();

#if !NO_CVARS
		if (HPC_DebugFootStrike->GetBool())
		{
			FString Message;
			Message += GetMesh()->GetOwner()->GetName();
			Message += "`s foot strike ";
			if (HitResult.Actor.IsValid())
				Message += HitResult.Actor.Get()->GetName();
			else
				Message += "nullptr";
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, Message);
		}
#endif
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

