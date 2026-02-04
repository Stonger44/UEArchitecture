// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupFuel.h"
#include "Ship.h"

// Sets default values for this component's properties
UPickupFuel::UPickupFuel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPickupFuel::BeginPlay()
{
	Super::BeginPlay();

	// Get Static Mesh, SM_Barrel
	StaticMesh = nullptr;
	StaticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (StaticMesh && StaticMesh->ComponentHasTag("SM_Barrel"))
	{
		StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &UPickupFuel::OnBeginOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StaticMesh not found for Pickup Fuel!!!"));
	}

	// Get Pivot
	Pivot = nullptr;
	TArray<USceneComponent*> SceneComps;
	GetOwner()->GetComponents<USceneComponent>(SceneComps);

	for (USceneComponent* Comp : SceneComps)
	{
		if (Comp && Comp->ComponentHasTag("Pivot"))
		{
			Pivot = Comp;

			float rotationSpeed = FMath::RandRange(RotationSpeedMin, RotationSpeedMax);
			float rotationDirection = FMath::RandBool() ? 1 : -1;
			
			RotationSpeed = rotationSpeed * rotationDirection;

			break;
		}
	}
}


// Called every frame
void UPickupFuel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Pivot)
	{
		FRotator NewRotation = Pivot->GetRelativeRotation();
		NewRotation.Roll += RotationSpeed * DeltaTime;
		Pivot->SetRelativeRotation(NewRotation);

		UE_LOG(LogTemp, Warning, TEXT("Fuel Rotation Speed: %f"), RotationSpeed);
	}
}

void UPickupFuel::OnBeginOverlap
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
		if (AShip* Ship = Cast<AShip>(OtherActor))
		{
			CollectPickup(Ship);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Pickup: Ship cast failed!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Pickup has been hit by something else!!"));
	}
}

void UPickupFuel::CollectPickup(AShip* Ship)
{
	Ship->AddFuel(PickupValue);
	UE_LOG(LogTemp, Warning, TEXT("Ship has collected a Pickup!!"));

	// Destroy Pickup
	if (AActor* Owner = GetOwner())
	{
		Owner->SetActorHiddenInGame(true);
		Owner->SetActorEnableCollision(false);
		Owner->Destroy();
	}
}