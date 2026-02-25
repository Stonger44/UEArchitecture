// Fill out your copyright notice in the Description page of Project Settings.


#include "LanderGameMode.h"
#include "Ship.h"
#include "LandingPad.h"
#include "Data/LevelData.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include <LanderPlayerController.h>

ALanderGameMode::ALanderGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Script/Engine.DataTable'/Game/DataTables/LevelData.LevelData'"));

	if (DataTableAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable found!"));

		LevelDataTable = DataTableAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DataTable NOT found!"));
	}

	DefaultPawnClass = AShip::StaticClass();
}

void ALanderGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Load Level Data
	if (LevelDataTable != nullptr)
	{
		FName RowName = FName(*FString::FromInt(GetCurrentLevelID()));
		static const FString ContextString(TEXT("GetLevel"));
		const FLevelData* Row = LevelDataTable->FindRow<FLevelData>(RowName, ContextString, true);

		if (Row != nullptr)
		{
			MaxFuel = Row->MaxFuel;
		}
	}

	// Subscribe to Ship Events
	AActor* ShipActor = UGameplayStatics::GetActorOfClass(GetWorld(), AShip::StaticClass());
	Ship = Cast<AShip>(ShipActor);
	if (Ship)
	{
		Ship->OnShipDestroyed.AddDynamic(this, &ALanderGameMode::HandleShipDestroyed);
		Ship->OnShipLanded.AddDynamic(this, &ALanderGameMode::HandleShipLanded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LanderGameMode: No Ship found in level!"));
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		LanderPlayerController = Cast<ALanderPlayerController>(PlayerController);
		if (LanderPlayerController == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("LanderPlayerController is null!"));
		}
	}
}

void ALanderGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Unsubscribe Ship Events
	if (Ship)
	{
		Ship->OnShipDestroyed.RemoveDynamic(this, &ALanderGameMode::HandleShipDestroyed);
		Ship->OnShipLanded.RemoveDynamic(this, &ALanderGameMode::HandleShipLanded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LanderGameMode: No Ship found in level!"));
	}
}

void ALanderGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ALanderGameMode::GetCurrentLevelID() const
{
	FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(this);
	for (auto& i : LevelDataTable->GetRowMap())
	{
		const FLevelData* Row = reinterpret_cast<const FLevelData*>(i.Value);
		if (Row && Row->LevelName == CurrentLevelName)
		{
			return Row->LevelID;
		}
	}

	return -1;
}

void ALanderGameMode::HandleShipDestroyed()
{
	GetWorldTimerManager().SetTimer(TriggerGameOverTimer, this, &ALanderGameMode::TriggerGameOver, 2.0f, false);
}

void ALanderGameMode::HandleShipLanded()
{
	GetWorldTimerManager().SetTimer(TriggerLevelSuccessTimer, this, &ALanderGameMode::TriggerLevelSuccess, 2.0f, false);
}

void ALanderGameMode::TriggerGameOver()
{
	if (LanderPlayerController)
	{
		LanderPlayerController->ShowGameOverMenu();
	}
}

void ALanderGameMode::TriggerLevelSuccess()
{
	if (LanderPlayerController)
	{
		LanderPlayerController->ShowLevelSuccessMenu();
	}
}

void ALanderGameMode::LoadNextLevel()
{
	int32 CurrentLevelID = GetCurrentLevelID();
	
	if (CurrentLevelID < 0)
	{
		return;
	}

	// Have Valid LevelID
	int32 NextLevelID = CurrentLevelID + 1;

	for (auto& i : LevelDataTable->GetRowMap())
	{
		const FLevelData* Row = reinterpret_cast<const FLevelData*>(i.Value);
		if (Row && Row->LevelID == NextLevelID)
		{
			UGameplayStatics::OpenLevel(this, Row->LevelName);

			return;
		}
	}

	// Last Level completed, no other Levels
	UE_LOG(LogTemp, Warning, TEXT("Congratulations! You've beaten the game!"))
}
