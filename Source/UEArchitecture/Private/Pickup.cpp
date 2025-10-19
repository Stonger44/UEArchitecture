// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Ship.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	SetRootComponent(PickupMesh);

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
	PickupMesh->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::OnBeginOverlap
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor && OtherActor->IsA(AShip::StaticClass()))
	{
		CollectPickup();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Pickup has been hit by something else!!"));
	}
}

void APickup::CollectPickup()
{
	UE_LOG(LogTemp, Warning, TEXT("Ship has collected a Pickup!!"));
}

