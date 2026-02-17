// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LanderPlayerController.generated.h"

UCLASS()
class UEARCHITECTURE_API ALanderPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameOverMenu();

protected:


private:

};
