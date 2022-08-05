// Bummoo Kim, 2022, Free license

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "DelegateEvent.generated.h"

USTRUCT(BlueprintType)
struct FIRSTPERSON_HORROR_API FFootHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditANywhere, BlueprintReadWrite)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHit;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FFootStrikeDelegate, const FFootHitData&);