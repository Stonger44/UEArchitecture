// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "LanderPlayerController.h"
#include "Ship.generated.h"

class UInputMappingContext;
class UInputAction;
class APad;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipReady, APad*, Pad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipLaunched, APad*, Pad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipLandingEvaluation, APad*, Pad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipLanded, APad*, Pad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipCrashedIntoPad, APad*, Pad);
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
	FOnShipReady OnShipReady;

	UPROPERTY()
	FOnShipLaunched OnShipLaunched;

	UPROPERTY()
	FOnShipLandingEvaluation OnShipLandingEvaluation;

	UPROPERTY()
	FOnShipLanded OnShipLanded;

	UPROPERTY()
	FOnShipCrashedIntoPad OnShipCrashedIntoPad;

	UPROPERTY()
	FOnShipDestroyed OnShipDestroyed;

	UFUNCTION()
	void TriggerExplode(bool ShipExplodedFromCrash);

	UFUNCTION()
	void AddFuel(float FuelToAdd);

	UFUNCTION(BlueprintCallable)
	float GetFuelPercent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
	float MaxInitialLandingRotation = 25;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Safety")
	float MaxLandingEvaluationRotation = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Safety")
	float MaxLandingSpeed = 200;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX | Thrusters")
	USceneComponent* Thrusters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX | Thrusters")
	UNiagaraComponent* Thruster1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX | Thrusters")
	UNiagaraComponent* Thruster2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX | Thrusters")
	UNiagaraComponent* Thruster3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX | Fire")
	USceneComponent* FireSmokeTrailPivot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX | Fire")
	UNiagaraComponent* FireSmokeTrail;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX | Explosions")
	UNiagaraSystem* NS_ExplosionBig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | VFX | Explosions")
	UNiagaraSystem* NS_ExplosionSmall;

	UPROPERTY(EditAnywhere, Category = "Ship | VFX | Explosions")
	TSubclassOf<UCameraShakeBase> ExplosionShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | SFX | Explosions")
	USoundCue* SC_ExplosionBig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | SFX | Explosions")
	USoundCue* SC_ExplosionSmall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | SFX | Thrusters")
	UAudioComponent* ThrusterAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | SFX | Fuel")
	UAudioComponent* FuelGlugAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ship | Camera")
	FVector LevelSuccessCameraOffset = FVector(-800, 0, 800);

	void Thrust(const FInputActionValue& inputValue);
	void Rotate(const FInputActionValue& inputValue);

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY()
	ALanderPlayerController* LanderPlayerController;

	UPROPERTY()
	APad* CurrentTouchdownTarget = nullptr;

	UPROPERTY()
	TArray<UNiagaraComponent*> ThrusterArray;

	float InitialLandingRotationThreshold;
	float LandingEvaluationRotationThreshold;

	FTimerHandle FuelRefillTimer;
	FTimerHandle LandingEvaluationTimer;
	FTimerHandle CameraPanTimer;

	bool bIsPanningCamera = false;
	FVector LevelSuccessCameraPosition;
	ECameraPanPhase CameraPanPhase = ECameraPanPhase::PanUp;

	void TriggerCameraPan();

	void CheckShipTouchdown();
	bool IsShipRotationSafe(float RotationThreshold) const;
	bool IsShipSpeedSafe() const;

	void ShipReady();
	void ShipLanded();
	void ShipCrashed();
	void ShipExploded(bool ShipExplodedFromCrash);

	void DisableShipControls();

	void CheckFuel(float DeltaTime);

	bool HasShipStoppedMoving() const;

	void RefillFuelTimer();

	void SpawnNiagaraSystem(UNiagaraSystem* NiagaraSystemToSpawn);

	void ShakeCamera();

	void ActivateThrusterEffects(bool ActivateThrusters);

	void EvaluateLanding();

	void PanCamera(float DeltaTime);
};
