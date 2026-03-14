// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "Ship.h"
#include "LanderGameMode.generated.h"

UCLASS()
class UEARCHITECTURE_API ALanderGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALanderGameMode();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Level")
	float GetMaxFuel() const { return MaxFuel; }

	UFUNCTION(BlueprintCallable, Category = "Level")
	FText GetLevelTitle() const { return LevelTitle; }

	UFUNCTION(BlueprintCallable)
	void RestartCurrentLevel();

	UFUNCTION(BlueprintCallable)
	void LoadNextLevel();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

protected:
	UPROPERTY()
	UDataTable* LevelDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	EEndPlayState EndPlayState;

	UFUNCTION()
	void HandleShipDestroyed();

	UFUNCTION()
	void HandleShipLanded(APad* CurrentTouchdownPad);

	UFUNCTION()
	void SetEndPlayTimer();

	UFUNCTION()
	void TriggerEndPlayState();

private:
	UPROPERTY()
	ALanderPlayerController* LanderPlayerController = nullptr;

	UPROPERTY()
	AShip* Ship = nullptr;

	FTimerHandle TriggerEndPlayTimer;

	float MaxFuel;

	FText LevelTitle;

	int32 GetCurrentLevelID() const;

	bool IsLastLevel() const;
};
