// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "SacredHeartGameMode.generated.h"

UCLASS(minimalapi)
class ASacredHeartGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASacredHeartGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerHeartRate)
		int gameModeHeartRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerHeartRate)
		int avgGameModeHeartRate;
};



