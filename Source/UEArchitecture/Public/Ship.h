// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "LanderPlayerController.h"
#include "Ship.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShipLanded);
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

	UPROPERTY()
	FOnShipLanded OnShipLanded;
	
	UFUNCTION()
	void TriggerExplode(bool ShipExplodedFromCrash);

	UFUNCTION()
	void AddFuel(float FuelToAdd);

	UFUNCTION(BlueprintCallable)
	float GetFuelPercent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Graphics")
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Graphics")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX")
	USceneComponent* FireSmokeTrailPivot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX")
	UNiagaraComponent* FireSmokeTrail;

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
	float MaxInitialLandingRotation = 25;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Safety")
	float MaxLandingEvaluationRotation = 50;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Fuel")
	float FuelRefillRate = 15;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship | Fuel")
	float FuelRefillRemaining;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship | Fuel")
	bool bIsThrusting = false;

	UPROPERTY(EditAnywhere, Category = "Ship | VFX")
	TSubclassOf<UCameraShakeBase> ExplosionShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX")
	UNiagaraSystem* NS_ExplosionBig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX")
	UNiagaraSystem* NS_ExplosionSmall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | SFX")
	USoundCue* SC_ExplosionBig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | SFX")
	USoundCue* SC_ExplosionSmall;

	void Thrust(const FInputActionValue& inputValue);
	void Rotate(const FInputActionValue& inputValue);

	void CheckShipTouchdown();
	bool IsShipRotationSafe(float RotationThreshold);
	bool IsShipSpeedSafe();

	void ShipReady();
	void ShipLanded();
	void ShipCrashed();
	void ShipExploded(bool ShipExplodedFromCrash);

	void DisableShipControls();

	void CheckFuel(float DeltaTime);

	bool HasShipStoppedMoving();

	void RefillFuelTimer();

	void SpawnNiagaraSystem(UNiagaraSystem* NiagaraSystemToSpawn, FVector LocationOffset = FVector::ZeroVector);

	void ShakeCamera();

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY()
	ALanderPlayerController* LanderPlayerController;

	UPROPERTY()
	AActor* CurrentTouchdownTarget = nullptr;

	float InitialLandingRotationThreshold;
	float LandingEvaluationRotationThreshold;

	FTimerHandle FuelRefillTimer;
	FTimerHandle LandingEvaluationTimer;
};
