// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

// Define enum for ship states
UENUM(BlueprintType) // lets you use it in Blueprints too
enum class EShipStatus : uint8
{
	IsReady		UMETA(DispayName = "Ready"),
	IsLaunched	UMETA(DisplayName = "Launched"),
	IsLanded	UMETA(DisplayName = "Landed"),
	IsCrashed	UMETA(DisplayName = "Crashed")
};

UCLASS()
class UEARCHITECTURE_API UEnums : public UObject
{
	GENERATED_BODY()
	
};
