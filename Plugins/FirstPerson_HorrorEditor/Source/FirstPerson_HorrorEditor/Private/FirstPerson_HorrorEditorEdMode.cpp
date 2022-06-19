// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPerson_HorrorEditorEdMode.h"
#include "FirstPerson_HorrorEditorEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FFirstPerson_HorrorEditorEdMode::EM_FirstPerson_HorrorEditorEdModeId = TEXT("EM_FirstPerson_HorrorEditorEdMode");

FFirstPerson_HorrorEditorEdMode::FFirstPerson_HorrorEditorEdMode()
{

}

FFirstPerson_HorrorEditorEdMode::~FFirstPerson_HorrorEditorEdMode()
{

}

void FFirstPerson_HorrorEditorEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FFirstPerson_HorrorEditorEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FFirstPerson_HorrorEditorEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FFirstPerson_HorrorEditorEdMode::UsesToolkits() const
{
	return true;
}




