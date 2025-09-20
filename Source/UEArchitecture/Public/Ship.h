// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Ship.generated.h"

class UInputMappingContext;
class UInputAction;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Graphics")
	UStaticMeshComponent* ShipMesh;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Landing Pad | Safety")
	float MaxLandingRotation = 12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Landing Pad | Safety")
	float MaxLandingSpeed = 250;

	void Thrust(const FInputActionValue& inputValue);
	void Rotate(const FInputActionValue& inputValue);

	bool IsShipRotationSafe();
	bool IsShipSpeedSafe();

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	bool bShipHasLanded = false;

	float LandingRotationThreshold;
};
