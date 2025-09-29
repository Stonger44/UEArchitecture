// Fill out your copyright notice in the Description page of Project Settings.


#include "KillVolume.h"
#include "Components/BoxComponent.h"
#include "Ship.h"

// Sets default values for this component's properties
UKillVolume::UKillVolume()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKillVolume::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		UBoxComponent* BoxComponent = Owner->FindComponentByClass<UBoxComponent>();
		if (BoxComponent)
		{
			BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UKillVolume::OnBeginOverlap);
		}
	}
	
}


// Called every frame
void UKillVolume::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKillVolume::OnBeginOverlap
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// Check to make sure OtherActor is Ship
	if (OtherActor && OtherActor != GetOwner() && OtherActor->IsA(AShip::StaticClass()))
	{
		AShip* Ship = Cast<AShip>(OtherActor);
		if (Ship)
		{
			UE_LOG(LogTemp, Error, TEXT("Ship out of bounds!!"));
			Ship->TriggerExplode();
		}
	}
}
