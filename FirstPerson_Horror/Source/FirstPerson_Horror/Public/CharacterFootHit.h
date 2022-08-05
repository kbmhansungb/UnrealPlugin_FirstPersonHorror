// Bummoo Kim, 2022, Free license

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DelegateEvent.h"
#include "CharacterFootHit.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterFootHit : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRSTPERSON_HORROR_API ICharacterFootHit
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CallFootStrike(const FFootHitData& FootHitEvent);
	virtual void CallFootStrike_Implementation(const FFootHitData& FootHitEvent) {}

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	USoundBase* GetFootSound(const FFootHitData& FootHitEvent);
};
