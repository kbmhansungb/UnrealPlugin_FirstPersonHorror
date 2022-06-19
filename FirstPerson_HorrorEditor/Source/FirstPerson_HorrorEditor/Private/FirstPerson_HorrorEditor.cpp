// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPerson_HorrorEditor.h"
#include "FirstPerson_HorrorEditorEdMode.h"

#define LOCTEXT_NAMESPACE "FFirstPerson_HorrorEditorModule"

void FFirstPerson_HorrorEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FFirstPerson_HorrorEditorEdMode>(FFirstPerson_HorrorEditorEdMode::EM_FirstPerson_HorrorEditorEdModeId, LOCTEXT("FirstPerson_HorrorEditorEdModeName", "FirstPerson_HorrorEditorEdMode"), FSlateIcon(), true);
}

void FFirstPerson_HorrorEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FFirstPerson_HorrorEditorEdMode::EM_FirstPerson_HorrorEditorEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFirstPerson_HorrorEditorModule, FirstPerson_HorrorEditor)