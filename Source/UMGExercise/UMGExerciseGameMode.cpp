// Copyright Epic Games, Inc. All Rights Reserved.

#include "UMGExerciseGameMode.h"
#include "UMGExerciseCharacter.h"
#include "UObject/ConstructorHelpers.h"


AUMGExerciseGameMode::AUMGExerciseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
