// Fill out your copyright notice in the Description page of Project Settings.


#include "LanderGameMode.h"
#include "Ship.h"
#include "LandingPad.h"
#include "Kismet/GameplayStatics.h"

ALanderGameMode::ALanderGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Script/Engine.DataTable'/Game/DataTables/LevelData.LevelData'"));

	if (DataTableAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable found!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DataTable NOT found!"));
	}
}

void ALanderGameMode::BeginPlay()
{
	Super::BeginPlay();

	AActor* ShipActor = UGameplayStatics::GetActorOfClass(GetWorld(), AShip::StaticClass());
	AShip* Ship = Cast<AShip>(ShipActor);

	if (Ship)
	{
		Ship->OnShipDestroyed.AddDynamic(this, &ALanderGameMode::HandleShipDestroyed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LanderGameMode: No Ship found in level!"));
	}

	AActor* LandingPadActor = UGameplayStatics::GetActorOfClass(GetWorld(), ALandingPad::StaticClass());
	ALandingPad* LandingPad = Cast<ALandingPad>(LandingPadActor);

	if (LandingPad)
	{
		LandingPad->OnShipLanded.AddDynamic(this, &ALanderGameMode::HandleShipLanded);
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

void ALanderGameMode::HandleShipDestroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("Ship has been destroyed! Restarting level..."));

	GetWorld()->GetTimerManager().SetTimer(LevelLoadTimer, this, &ALanderGameMode::RestartCurrentLevel, 3.0f, false);
}

void ALanderGameMode::RestartCurrentLevel()
{
	// Restart Level
	FName CurrentlevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, CurrentlevelName, false);
}

void ALanderGameMode::HandleShipLanded()
{
	UE_LOG(LogTemp, Warning, TEXT("Ship has landed! Loading next level..."));

	GetWorld()->GetTimerManager().SetTimer(LevelLoadTimer, this, &ALanderGameMode::LoadNextLevel, 3.0f, false);
}

void ALanderGameMode::LoadNextLevel()
{
	//TODO: Load next level instead of restart
	// Restart Level
	FName CurrentlevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, CurrentlevelName, false);
}