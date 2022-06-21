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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AHorrorPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHorrorPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AHorrorPlayerCharacter::MoveForward(float Scale)
{
	AddMovementInput(GetCapsuleComponent()->GetForwardVector(), Scale);
}

void AHorrorPlayerCharacter::MoveRight(float Scale)
{
	AddMovementInput(GetCapsuleComponent()->GetRightVector(), Scale);
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

