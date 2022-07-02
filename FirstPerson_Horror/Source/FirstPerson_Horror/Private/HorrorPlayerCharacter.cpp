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

	if (IsInDOF)
		UpdateFocusEvent(DeltaTime);

	if (IsInDesireCameraRotation)
		UpdateCameraRotationToDesire(DeltaTime);
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

void AHorrorPlayerCharacter::AddControllerRotator(const FRotator& Rotator)
{
	AHorrorPlayerCharacter::AddControllerPitchInput(Rotator.Pitch);
	AHorrorPlayerCharacter::AddControllerYawInput(Rotator.Yaw);
}

void AHorrorPlayerCharacter::CallFootStrike(FName SocketName, float Speed, ECollisionChannel TraceChannel, const FVector& Offset)
{
	FFootHitEvent FootHitEvent{};

	if (GetMesh()->SkeletalMesh)
	{
		const FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
		const FVector Start = SocketLocation + Offset;
		const FVector End = SocketLocation - Offset;

		TraceFoot(TraceChannel, Start, End, FootHitEvent);
	}

	FootHitEvent.Speed = Speed;

	if (FootStrikeDelegate.IsBound())
		FootStrikeDelegate.Broadcast(FootHitEvent);
}

void AHorrorPlayerCharacter::TraceFoot(ECollisionChannel TraceChannel, const FVector& Start, const FVector& End, FFootHitEvent& FootHitEvent)
{
#if !NO_CVARS
	static const auto HPC_DebugFootStrike = IConsoleManager::Get().FindConsoleVariable(TEXT("HPC.DebugFootStrike"));
#endif

	TArray<AActor*> TraceIgnore;
	if (GetMesh()->GetOwner())
	{
		TraceIgnore.Add(GetMesh()->GetOwner());
	}

	FootHitEvent.IsHit = UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		Start,
		End,
		5.f, 0.0f,
		UEngineTypes::ConvertToTraceType(TraceChannel),
		IsFootHitComplex,
		TraceIgnore,
#if ENABLE_DRAW_DEBUG && !NO_CVARS
		HPC_DebugFootStrike->GetBool() ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
#else
		EDrawDebugTrace::None,
#endif
		FootHitEvent.HitResult,
		false
	);

#if !NO_CVARS
	if (HPC_DebugFootStrike->GetBool())
	{
		FString Message;
		Message += GetMesh()->GetOwner()->GetName();
		Message += "`s foot strike ";
		if (FootHitEvent.HitResult.Actor.IsValid())
			Message += FootHitEvent.HitResult.Actor.Get()->GetName();
		else
			Message += "nullptr";
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, Message);
	}
#endif
}

void AHorrorPlayerCharacter::RotateCameraToLookAt(const FVector& NewLookAt, float Duration)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	const FRotator& DesireRotation = UKismetMathLibrary::FindLookAtRotation(DefaultCamera->GetComponentLocation(), NewLookAt);
	const FRotator& CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	DesireCameraRotator = (DesireRotation - CameraRotation);
	DesireCameraRotator.Normalize();

	DesireCameraRotator.Pitch /= PlayerController->InputPitchScale;
	DesireCameraRotator.Yaw /= PlayerController->InputYawScale;

	if (Duration > FLT_EPSILON)
	{
		CameraLookAtSpeed = 1.0f / Duration;
		StartRotateCamera();
	}
	else
	{
		AddControllerRotator(DesireCameraRotator);
	}
}

void AHorrorPlayerCharacter::StartRotateCamera()
{
	IsInDesireCameraRotation = true;
	CameraLookAtPrograss = 0.f;
}

void AHorrorPlayerCharacter::EndRotateCamera()
{
	IsInDesireCameraRotation = false;
}

void AHorrorPlayerCharacter::UpdateCameraRotationToDesire(float DeletaTime)
{
	float Delta = DeletaTime * CameraLookAtSpeed;
	CameraLookAtPrograss += Delta;

	if (CameraLookAtPrograss >= 1.f)
	{
		Delta -= (CameraLookAtPrograss - 1.f);
		EndRotateCamera();
	}

	AddControllerRotator(DesireCameraRotator * Delta);
}

void AHorrorPlayerCharacter::StartDOF()
{
	IsInDOF = true;
	DOFPrograss = 0.f;
}

void AHorrorPlayerCharacter::EndDOF()
{
	IsInDOF = false;
}

void AHorrorPlayerCharacter::DOFIn(const FVector& Location, float Duration)
{
	if (Duration <= FLT_EPSILON)
		return;

	DOFSpeed = 1.f / Duration;
	IsDOFIn = true;

	if (DefaultCamera->PostProcessSettings.DepthOfFieldFocalDistance == 0.f)
	{
		DefaultCamera->PostProcessSettings.DepthOfFieldFocalDistance = (Location - GetActorLocation()).Size();
	}
	else
	{
		DOFDistanceVector
			= (Location - GetActorLocation()).Size()
			- DefaultCamera->PostProcessSettings.DepthOfFieldFocalDistance;
	}

	StartDOF();
}

void AHorrorPlayerCharacter::DOFOut(float Duration)
{
	DOFSpeed = 1.f / Duration;
	IsDOFIn = false;

	DOFDistanceVector = 0.f;

	StartDOF();
}

void AHorrorPlayerCharacter::UpdateFocusEvent(float DeltaTime)
{
	float Delta = DeltaTime * DOFSpeed;
	DOFPrograss += DeltaTime;
	DOFWeight = FMath::Clamp(IsDOFIn ? DOFWeight + Delta : DOFWeight - Delta, 0.f, 1.f);

	if (DOFPrograss >= 1.f)
	{
		Delta -= DOFPrograss - 1.f;
		EndDOF();
	}

	DefaultCamera->PostProcessSettings.DepthOfFieldDepthBlurRadius = DOFWeight * SetCircleDOFFromFocus;
	DefaultCamera->PostProcessSettings.DepthOfFieldFocalDistance += DOFDistanceVector * Delta;
}

void AHorrorPlayerCharacter::FocusIn(const FVector& Location, float Duration)
{
	DOFIn(Location, Duration);
	RotateCameraToLookAt(Location, Duration);
}

void AHorrorPlayerCharacter::FocusOut(float Duration)
{
	DOFOut(Duration);
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

