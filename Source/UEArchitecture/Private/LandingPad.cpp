// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingPad.h"
#include "Ship.h"
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

}

// Called every frame
void ALandingPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALandingPad::NotifyHit
(
	class UPrimitiveComponent* MyComp,
	AActor* Other,
	class UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (!bShipHasLanded && Other && Other != this && Other->IsA(AShip::StaticClass()))
	{
		AShip* Ship = Cast<AShip>(Other);
		if (Ship && Ship->CurrentShipStatus() == EShipStatus::Landed)
		{
			bShipHasLanded = true;
			UE_LOG(LogTemp, Warning, TEXT("Starting Next Level!"));
			TriggerNextLevel();
		}
	}
}

void ALandingPad::TriggerNextLevel()
{
	GetWorld()->GetTimerManager().SetTimer(LevelLoadTimer, this, &ALandingPad::LoadNextLevel, 3.0f, false);
}

void ALandingPad::LoadNextLevel()
{
	// Restart Level
	FName CurrentlevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, CurrentlevelName, false);
}
