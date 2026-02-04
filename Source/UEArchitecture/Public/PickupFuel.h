// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupFuel.generated.h"

UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class UEARCHITECTURE_API UPickupFuel : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupFuel();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* Pivot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PickupValue = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeedMin = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeedMax = 20;

	UFUNCTION()
	void OnBeginOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void CollectPickup(AShip* Ship);

private:
	float RotationSpeed;
};
