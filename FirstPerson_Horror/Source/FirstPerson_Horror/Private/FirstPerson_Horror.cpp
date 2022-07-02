// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPerson_Horror.h"

#define LOCTEXT_NAMESPACE "FFirstPerson_HorrorModule"

void FFirstPerson_HorrorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FFirstPerson_HorrorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFirstPerson_HorrorModule, FirstPerson_Horror)


#if !NO_CVARS
#include <HAL/IConsoleManager.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerController.h>
#include <Components/SkeletalMeshComponent.h>
#include "HorrorPlayerCharacter.h"

TAutoConsoleVariable<bool> DebugFootStrike(TEXT("Horror.DebugFootStrike"), false, TEXT("Debug FootStrike."), ECVF_Default);

FAutoConsoleCommand CCmd_ToogleCharacterVisibility(
    TEXT("Horror.ToggleCharacterVisibility"),
    TEXT("If first person character is vaild."),
    FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Params)
        {
            UWorld* WorldContext = nullptr;
            for (FWorldContext const& Context : GEngine->GetWorldContexts())
            {
                switch (Context.WorldType)
                {
                case EWorldType::PIE:
                    WorldContext = Context.World();
                    break;
                case EWorldType::Game:
                    WorldContext = Context.World();
                    break;
                }

                if (WorldContext) break;
            }
            if (!WorldContext) return;

            APlayerController* Controller = GEngine->GetFirstLocalPlayerController(WorldContext);
            if (!Controller) return;

            AHorrorPlayerCharacter* Character = Cast<AHorrorPlayerCharacter>(Controller->GetPawn());
            if (!Character) return;

            Character->GetMesh()->ToggleVisibility(false);
        }),
    ECVF_Default);

#endif