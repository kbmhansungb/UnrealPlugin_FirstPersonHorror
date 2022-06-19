// Bummoo Kim, 2022, Free license

#include "FootResponseDataAsset.h"
#include "GameFramework/PlayerController.h"

void UFootResponseDataAsset::FootResponseSpacificSurfaceType(APlayerController* PlayerController, const FHitResult& HitResult, const FVector& Velocity, const FFootResponseStruct& FootResponseStruct)
{
	// Main player controller
	if (PlayerController != nullptr)
	{
		PlayerController->ClientPlayCameraShake(FootResponseStruct.CameraShake, );
	}
}
