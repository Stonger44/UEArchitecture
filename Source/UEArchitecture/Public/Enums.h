// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

// Define enum for ship states
UENUM(BlueprintType) // lets you use it in Blueprints too
enum class EShipStatus : uint8
{
	Ready		UMETA(DisplayName = "Ready"),
	Launched	UMETA(DisplayName = "Launched"),
	Landed		UMETA(DisplayName = "Landed"),
	Crashed		UMETA(DisplayName = "Crashed")
};

//UENUM(BlueprintType)
//enum class EGameState : uint8
//{
//	MainMenu    UMETA(DisplayName = "Main Menu"),
//	Playing     UMETA(DisplayName = "Playing"),
//	Paused      UMETA(DisplayName = "Paused"),
//	GameOver    UMETA(DisplayName = "Game Over")
//};
