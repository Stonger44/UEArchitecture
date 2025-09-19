// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingPad.h"
#include "Ship.h"

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

	// Calculate Tilt Threshold
	RotationThreshold = FMath::Cos(FMath::DegreesToRadians(MaxLandingRotation));
	UE_LOG(LogTemp, Warning, TEXT("Rotation Threshold: %f"), RotationThreshold);
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

	// Add Landing safety Checks
	// -Is Ship rotation within threshold?
	// -Is Ship speed within threshold?

	if (!bShipHasLanded && Other && Other != this && Other->IsA(AShip::StaticClass()))
	{
		bool bIsShipVelocitySafe = IsShipSpeedSafe(Other);
		bool bIsShipRotationSafe = IsShipRotationSafe(Other);

		if (bIsShipVelocitySafe && bIsShipRotationSafe)
		{
			bShipHasLanded = true;
			UE_LOG(LogTemp, Warning, TEXT("SAAAAFE!!!!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CRAAAASH!!!!"));
		}
		
	}
}

bool ALandingPad::IsShipSpeedSafe(AActor* Ship)
{
	FVector ShipVelocity = Ship->GetVelocity();

	float shipSpeed = ShipVelocity.Size();

	bool bIsShipSpeedSafe = shipSpeed <= MaxLandingSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Ship speed: %f"), shipSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Max Landing speed: %f"), MaxLandingSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Ship Speed is Safe: %s"), bIsShipSpeedSafe ? TEXT("True") : TEXT("False"));

	return bIsShipSpeedSafe;
}

bool ALandingPad::IsShipRotationSafe(AActor* Ship)
{
	FVector shipUpVector = Ship->GetActorUpVector();

	// DotProduct for ship alignment
	float shipUpAlignment = FVector::DotProduct(shipUpVector, FVector::UpVector);
	UE_LOG(LogTemp, Warning, TEXT("Ship Up Alignment: %f"), shipUpAlignment);

	// Compare ship alignment with threshold
	bool bIsShipRotationSafe = shipUpAlignment >= RotationThreshold;
	UE_LOG(LogTemp, Warning, TEXT("Rotation Threshold: %f"), RotationThreshold);
	UE_LOG(LogTemp, Warning, TEXT("Ship Rotation is Safe: %s"), bIsShipRotationSafe ? TEXT("True") : TEXT("False"));

	return bIsShipRotationSafe;
}
