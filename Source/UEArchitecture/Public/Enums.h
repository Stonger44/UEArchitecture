// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

// Define enum for ship states
UENUM(BlueprintType) // lets you use it in Blueprints too
enum class EShipStatus : uint8
{
	Ready				UMETA(DisplayName = "Ready"),
	Launched			UMETA(DisplayName = "Launched"),
	LandingEvaluation	UMETA(DisplayName = "LandingEvaluation"),
	Landed				UMETA(DisplayName = "Landed"),
	Crashed				UMETA(DisplayName = "Crashed"),
	Exploded			UMETA(DisplayName = "Exploded")
};

UENUM(BlueprintType)
enum class EEndPlayState : uint8
{
	GameOver		UMETA(DisplayName = "GameOver"),
	LevelSuccess	UMETA(DisplayName = "LevelSuccess"),
	GameComplete    UMETA(DisplayName = "GameComplete")
};
