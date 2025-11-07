// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Ship.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShipDestroyed);

UCLASS()
class UEARCHITECTURE_API AShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyHit
	(
		class UPrimitiveComponent* MyComp,
		AActor* Other,
		class UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;

	EShipStatus CurrentShipStatus() const { return ShipStatus; }

	UPROPERTY()
	FOnShipDestroyed OnShipDestroyed;

	UFUNCTION()
	void TriggerCrash();
	
	UFUNCTION()
	void TriggerExplode();

	UFUNCTION()
	void AddFuel(float FuelToAdd);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Graphics")
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Graphics")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Movement")
	float ThrustStrength = 3000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Movement")
	float TorqueStrength = 10;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Ship | Input")
	UInputMappingContext* ShipMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Ship | Input")
	UInputAction* ThrustAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Ship | Input")
	UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Safety")
	float MaxLandingRotation = 25;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Safety")
	float MaxLandingSpeed = 300;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship | Status")
	EShipStatus ShipStatus = EShipStatus::Ready;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Fuel")
	float MaxFuel = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship | Fuel")
	float Fuel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Fuel")
	float FuelDrainRate = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship | Fuel")
	bool bIsThrusting = false;

	void Thrust(const FInputActionValue& inputValue);
	void Rotate(const FInputActionValue& inputValue);

	void CheckShipLanding(AActor* Other);
	bool IsShipRotationSafe();
	bool IsShipSpeedSafe();

	void ShipReady();
	void ShipLanded();
	void ShipCrashed();
	void ShipExploded();

	void DisableShipControls();

	void CheckFuel(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	float LandingRotationThreshold;
};
