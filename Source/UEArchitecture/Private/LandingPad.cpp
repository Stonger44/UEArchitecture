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
	RotationThreshold = FMath::Cos(FMath::DegreesToRadians(MaxShipRotation));
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
		FVector shipUpVector = Other->GetActorUpVector();
		
		// DotProduct for ship alignment
		float shipUpAlignment = FVector::DotProduct(shipUpVector, FVector::UpVector);
		UE_LOG(LogTemp, Warning, TEXT("Ship Up Alignment: %f"), shipUpAlignment);

		// Compare ship alignment with threshold
		bool bIsShipRotationSafe = shipUpAlignment >= RotationThreshold;
		UE_LOG(LogTemp, Warning, TEXT("Rotation Threshold: %f"), RotationThreshold);
		UE_LOG(LogTemp, Warning, TEXT("Ship Rotation is Safe: %s"), bIsShipRotationSafe ? TEXT("True") : TEXT("False"));

		if (bIsShipRotationSafe)
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
