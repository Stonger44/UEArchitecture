// Fill out your copyright notice in the Description page of Project Settings.


#include "Pad.h"
#include "Ship.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APad::APad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	SetRootComponent(DefaultSceneRoot);

	Pad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pad"));
	Pad->SetupAttachment(DefaultSceneRoot);

	PadLights = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadLights"));
	PadLights->SetupAttachment(Pad);

	PadLight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PadLight1"));
	PadLight1->SetupAttachment(PadLights);

	PadLight2 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PadLight2"));
	PadLight2->SetupAttachment(PadLights);

	PadLight3 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PadLight3"));
	PadLight3->SetupAttachment(PadLights);

	PadLight4 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PadLight4"));
	PadLight4->SetupAttachment(PadLights);
}

// Called when the game starts or when spawned
void APad::BeginPlay()
{
	Super::BeginPlay();

	// Subscribe to Ship Events
	AActor* ShipActor = UGameplayStatics::GetActorOfClass(GetWorld(), AShip::StaticClass());
	Ship = Cast<AShip>(ShipActor);
	if (Ship)
	{
		Ship->OnShipLanded.AddDynamic(this, &APad::HandleShipLanded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Pad: No Ship found in level!"));
	}

	PadLightArray.Add(PadLight1);
	PadLightArray.Add(PadLight2);
	PadLightArray.Add(PadLight3);
	PadLightArray.Add(PadLight4);

	for (auto* Light : PadLightArray)
	{
		Light->SetIntensity(250000.0f);
		Light->SetLightColor(White);
	}
}

void APad::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unsubscribe from Ship Events
	if (Ship)
	{
		Ship->OnShipLanded.RemoveDynamic(this, &APad::HandleShipLanded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Pad: No Ship found in level!"));
	}
}

// Called every frame
void APad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APad::HandleShipReady()
{

}

void APad::HandleShipLaunched()
{

}

void APad::HandleShipLandingEvaluation()
{

}

void APad::HandleShipLanded(APad* CurrentTouchdownPad)
{
	for (auto* Light : PadLightArray)
	{
		Light->SetLightColor(Green);
	}

	UE_LOG(LogTemp, Warning, TEXT("Ship landed on: %s"), *CurrentTouchdownPad->GetName());
}
