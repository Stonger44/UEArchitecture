// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LanderGameMode.generated.h"

UCLASS()
class UEARCHITECTURE_API ALanderGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void RestartCurrentLevel();

	void LoadNextLevel();

protected:


private:
	UFUNCTION()
	void HandleShipDestroyed();

	UFUNCTION()
	void HandleShipLanded();

	FTimerHandle LevelLoadTimer;

};
