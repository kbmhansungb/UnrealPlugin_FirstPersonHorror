// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FirstPersonHorrorHUD.generated.h"

UCLASS()
class AFirstPersonHorrorHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFirstPersonHorrorHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

