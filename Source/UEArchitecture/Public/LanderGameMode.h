// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "LanderGameMode.generated.h"

UCLASS()
class UEARCHITECTURE_API ALanderGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALanderGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	//float MaxFuel;

	UFUNCTION(BlueprintCallable, Category = "Level")
	float GetMaxFuel() const { return MaxFuel; }

protected:
	UPROPERTY()
	UDataTable* LevelDataTable;

	UFUNCTION()
	void HandleShipDestroyed();

	UFUNCTION()
	void HandleShipLanded();

	UFUNCTION()
	void RestartCurrentLevel();

	UFUNCTION()
	void LoadNextLevel();

private:
	FTimerHandle LevelLoadTimer;

	float MaxFuel;

	int32 GetCurrentLevelID() const;
};
