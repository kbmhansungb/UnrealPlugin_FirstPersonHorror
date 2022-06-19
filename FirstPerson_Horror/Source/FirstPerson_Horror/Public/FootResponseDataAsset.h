// Bummoo Kim, 2022, Free license

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "FootResponseDataAsset.generated.h"

class APlayerController;
class UCamerashake;

USTRUCT(BlueprintType)
struct FIRSTPERSON_HORROR_API FFootResponseStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraShake* CameraShake;
};

/**
 * 
 */
UCLASS(Blueprintable)
class FIRSTPERSON_HORROR_API UFootResponseDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	* 피직스 머티리얼을 추가한 후 블루프린트에서 switch를 구현해야 합니다.
	*/
	UFUNCTION(Category = "HitResponse", BlueprintImplementableEvent)
	void ResponseHit(const FHitResult& HitResult);

protected:
	UPROPERTY(Category = "HitResponse", VisibleAnywhere, BlueprintReadOnly)
	FFootResponseStruct DefaultSurface;

	void FootResponseSpacificSurfaceType(APlayerController* PlayerController, const FHitResult& HitResult, const FVector& Velocity, const FFootResponseStruct& FootResponseStruct);
};