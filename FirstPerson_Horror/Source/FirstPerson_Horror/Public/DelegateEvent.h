// Bummoo Kim, 2022, Free license

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "DelegateEvent.generated.h"

class UPhysicalMaterial;

USTRUCT(BlueprintType)
struct FIRSTPERSON_HORROR_API FFootHitEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHit;
};

DECLARE_MULTICAST_DELEGATE_OneParam(
FFootStrikeDelegate,
const FFootHitEvent&
);

