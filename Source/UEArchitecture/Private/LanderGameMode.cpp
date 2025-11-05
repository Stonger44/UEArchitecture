// Fill out your copyright notice in the Description page of Project Settings.


#include "LanderGameMode.h"
#include "Ship.h"
#include "LandingPad.h"
#include "Data/LevelData.h"
#include "Kismet/GameplayStatics.h"

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

	// Subscribe to OnShipDestroyed Event
	AActor* ShipActor = UGameplayStatics::GetActorOfClass(GetWorld(), AShip::StaticClass());
	Ship = Cast<AShip>(ShipActor);
	if (Ship)
	{
		Ship->OnShipDestroyed.AddDynamic(this, &ALanderGameMode::HandleShipDestroyed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LanderGameMode: No Ship found in level!"));
	}

	// Subscribe to OnShipLanded Event
	AActor* LandingPadActor = UGameplayStatics::GetActorOfClass(GetWorld(), ALandingPad::StaticClass());
	LandingPad = Cast<ALandingPad>(LandingPadActor);
	if (LandingPad)
	{
		LandingPad->OnShipLanded.AddDynamic(this, &ALanderGameMode::HandleShipLanded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LanderGameMode: No LandingPad found in level!"));
	}

	// Enable only UI controls on MainMenu Level
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController)
	{
		FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(this);
		if (CurrentLevelName == "MainMenu")
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeUIOnly());
		}
		else
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void ALanderGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Unsubscribe from OnShipDestroyed Event
	if (Ship)
	{
		Ship->OnShipDestroyed.RemoveDynamic(this, &ALanderGameMode::HandleShipDestroyed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LanderGameMode: No Ship found in level!"));
	}

	// Unsubscribe from OnShipLanded Event
	if (LandingPad)
	{
		LandingPad->OnShipLanded.RemoveDynamic(this, &ALanderGameMode::HandleShipLanded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LanderGameMode: No LandingPad found in level!"));
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
	UE_LOG(LogTemp, Warning, TEXT("Ship has been destroyed! Restarting level..."));

	GetWorld()->GetTimerManager().SetTimer(LevelLoadTimer, this, &ALanderGameMode::RestartCurrentLevel, 3.0f, false);
}

void ALanderGameMode::HandleShipLanded()
{
	UE_LOG(LogTemp, Warning, TEXT("Ship has landed! Loading next level..."));

	GetWorld()->GetTimerManager().SetTimer(LevelLoadTimer, this, &ALanderGameMode::LoadNextLevel, 3.0f, false);
}

void ALanderGameMode::RestartCurrentLevel()
{
	FName CurrentlevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, CurrentlevelName);
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
