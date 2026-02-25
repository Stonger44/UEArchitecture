// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingPad.h"
#include "Ship.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALandingPad::ALandingPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALandingPad::BeginPlay()
{
	Super::BeginPlay();

	// Subscribe to Ship Events
	AActor* ShipActor = UGameplayStatics::GetActorOfClass(GetWorld(), AShip::StaticClass());
	Ship = Cast<AShip>(ShipActor);
	if (Ship)
	{
		Ship->OnShipLanded.AddDynamic(this, &ALandingPad::HandleShipLanded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LandingPad: No Ship found in level!"));
	}
}

void ALandingPad::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Ship)
	{
		Ship->OnShipLanded.RemoveDynamic(this, &ALandingPad::HandleShipLanded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LandingPad: No Ship found in level!"));
	}
}

// Called every frame
void ALandingPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALandingPad::HandleShipLanded()
{
	UE_LOG(LogTemp, Warning, TEXT("The ship has successfully landed! Fire the fireworks!!! Landing pad lights are green!"));
}
