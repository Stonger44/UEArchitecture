// Fill out your copyright notice in the Description page of Project Settings.


#include "Pad.h"

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
	
}

// Called every frame
void APad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

