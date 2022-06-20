// Bummoo Kim, 2022, Free license

#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "DelegateEvent.generated.h"

class UPhysicalMaterial;

USTRUCT(BlueprintType)
struct FIRSTPERSON_HORROR_API FFootHitEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HitLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootVelocityLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicalMaterial* HitPhysicsMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHit;
};

DECLARE_MULTICAST_DELEGATE_OneParam(
FFootStrikeDelegate,
const FFootHitEvent&
);

