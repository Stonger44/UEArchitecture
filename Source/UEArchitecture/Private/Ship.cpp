// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "LandingPad.h"
#include "LaunchPad.h"
#include "LanderGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include <NiagaraFunctionLibrary.h>

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	SetRootComponent(ShipMesh);

	// VisualMesh exists because the StaticMesh for the Ship asset was rotated by 90 degrees
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(ShipMesh);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(ShipMesh);
	CameraBoom->TargetArmLength = 2000;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	FireSmokeTrailPivot = CreateDefaultSubobject<USceneComponent>(TEXT("FireSmokeTrailPivot"));
	FireSmokeTrailPivot->SetupAttachment(ShipMesh);
	FireSmokeTrailPivot->SetRelativeLocation(FVector(-200.f, -50.f, 100.f));
	FireSmokeTrailPivot->SetRelativeRotation(FRotator::ZeroRotator);
	FireSmokeTrailPivot->SetUsingAbsoluteRotation(true);

	FireSmokeTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireSmokeTrail"));
	FireSmokeTrail->SetupAttachment(FireSmokeTrailPivot);
	FireSmokeTrail->SetRelativeLocation(FVector::ZeroVector);
	FireSmokeTrail->SetRelativeRotation(FRotator::ZeroRotator);
	FireSmokeTrail->bAutoActivate = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ShipMesh->SetAngularDamping(5);

}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	LanderPlayerController = Cast<ALanderPlayerController>(GetController());
	if (LanderPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* enhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LanderPlayerController->GetLocalPlayer()))
		{
			enhancedInputLocalPlayerSubsystem->AddMappingContext(ShipMappingContext, 0);
		}
	}

	// Calculate Landing Rotation Thresholds
	InitialLandingRotationThreshold = FMath::Cos(FMath::DegreesToRadians(MaxInitialLandingRotation));
	UE_LOG(LogTemp, Warning, TEXT("Landing Rotation Threshold: %f"), InitialLandingRotationThreshold);

	LandingEvaluationRotationThreshold = FMath::Cos(FMath::DegreesToRadians(MaxLandingEvaluationRotation));
	UE_LOG(LogTemp, Warning, TEXT("Landing Evaluation Rotation Threshold: %f"), LandingEvaluationRotationThreshold);

	ShipStatus = EShipStatus::Ready;

	ALanderGameMode* LanderGM = Cast<ALanderGameMode>(UGameplayStatics::GetGameMode(this));
	if (LanderGM)
	{
		MaxFuel = LanderGM->GetMaxFuel();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Ship.cpp: LanderGameMode NOT FOUND!!"))
	}

	Fuel = MaxFuel;
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFuel(DeltaTime);

	if (FireSmokeTrail->IsActive())
	{
		FVector Offset = FVector(-150.f, -50.f, 100.f);
		FireSmokeTrail->SetWorldLocation(GetActorLocation() + Offset);
		FireSmokeTrail->SetWorldRotation(FRotator::ZeroRotator);
	}

	if (ShipStatus == EShipStatus::Crashed)
	{
		if (HasShipStoppedMoving())
		{
			ShipExploded(true);
		}
	}

	if (ShipStatus == EShipStatus::LandingEvaluation)
	{
		if (HasShipStoppedMoving())
		{
			if (IsShipRotationSafe(LandingEvaluationRotationThreshold))
			{
				if (CurrentTouchdownTarget)
				{
					if (CurrentTouchdownTarget->IsA(ALaunchPad::StaticClass()))
					{
						ShipReady();
					}
					
					if (CurrentTouchdownTarget->IsA(ALandingPad::StaticClass()))
					{
						ShipLanded();
					}
				}
				
			}
			else
			{
				ShipCrashed();
			}
		}
	}
}

void AShip::NotifyHit
(
	class UPrimitiveComponent* MyComp,
	AActor* Other,
	class UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (ShipStatus == EShipStatus::Launched)
	{
		if (Other && Other != this)
		{
			if (Other->IsA(ALaunchPad::StaticClass()) || Other->IsA(ALandingPad::StaticClass()))
			{
				CurrentTouchdownTarget = Other;
				CheckShipTouchdown();
			}
			else
			{
				ShipCrashed();
			}
		}
	}
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Thrust Actions to Function
		EnhancedInputComponent->BindAction(ThrustAction, ETriggerEvent::Triggered, this, &AShip::Thrust);
		EnhancedInputComponent->BindAction(ThrustAction, ETriggerEvent::Completed, this, &AShip::Thrust);

		//Bind Rotate Actions to Function
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AShip::Rotate);
	}

}

void AShip::Thrust(const FInputActionValue& InputValue)
{
	bIsThrusting = InputValue.Get<bool>();

	if (bIsThrusting && Fuel > 0)
	{
		if (ShipStatus == EShipStatus::Ready || ShipStatus == EShipStatus::LandingEvaluation)
		{
			ShipStatus = EShipStatus::Launched;
			CurrentTouchdownTarget = nullptr;
		}

		const FVector thrust = GetActorUpVector() * ThrustStrength;

		ShipMesh->AddForce(thrust, NAME_None, true);
	}
}

void AShip::Rotate(const FInputActionValue& InputValue)
{
	if (float currentValue = InputValue.Get<float>())
	{
		const FVector torque = FVector(-currentValue, 0, 0) * TorqueStrength;

		ShipMesh->AddTorqueInRadians(torque, NAME_None, true);
	}
}

void AShip::CheckFuel(float DeltaTime)
{
	if (bIsThrusting)
	{
		Fuel -= FuelDrainRate * DeltaTime;

		if (Fuel <= 0)
		{
			Fuel = 0;
			bIsThrusting = false;
		}
	}

	//// For Development
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, INDEFINITELY_LOOPING_DURATION, FColor::Green, FString::Printf(TEXT("Fuel: %.2f / %.2f"), Fuel, MaxFuel));
	//}
}

void AShip::AddFuel(float FuelToAdd)
{
	FuelRefillRemaining = FuelToAdd;

	if (!GetWorldTimerManager().IsTimerActive(FuelRefillTimer))
	{
		GetWorldTimerManager().SetTimer(
			FuelRefillTimer,
			this,
			&AShip::RefillFuelTimer,
			0.01f,
			true
		);
	}
}

void AShip::RefillFuelTimer()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float FuelToAdd = FuelRefillRate * DeltaTime;

	float FuelAdded = FMath::Min(FuelToAdd, FuelRefillRemaining);

	Fuel += FuelAdded;
	FuelRefillRemaining -= FuelAdded;

	Fuel = FMath::Min(Fuel, MaxFuel);

	if (FuelRefillRemaining <= 0 || Fuel >= MaxFuel)
	{
		GetWorldTimerManager().ClearTimer(FuelRefillTimer);
	}
}

float AShip::GetFuelPercent()
{
	return Fuel / MaxFuel;
}

void AShip::CheckShipTouchdown()
{
	if (IsShipSpeedSafe() && IsShipRotationSafe(InitialLandingRotationThreshold))
	{
		// After ship touches down, check to see if ship has fallen over
		ShipStatus = EShipStatus::LandingEvaluation;
	}
	else
	{
		ShipCrashed();
	}
}

bool AShip::IsShipSpeedSafe()
{
	float shipSpeed = this->GetVelocity().Length();

	UE_LOG(LogTemp, Warning, TEXT("Ship speed: %f"), shipSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Max Landing speed: %f"), MaxLandingSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Ship Speed is Safe: %s"), shipSpeed <= MaxLandingSpeed ? TEXT("True") : TEXT("False"));

	return shipSpeed <= MaxLandingSpeed;
}

bool AShip::IsShipRotationSafe(float RotationThreshold)
{
	FVector shipUpVector = this->GetActorUpVector();

	// DotProduct for ship alignment
	float shipUpAlignment = FVector::DotProduct(shipUpVector, FVector::UpVector);
	UE_LOG(LogTemp, Warning, TEXT("Ship Up Alignment: %f"), shipUpAlignment);

	// Compare ship alignment with threshold
	UE_LOG(LogTemp, Warning, TEXT("Rotation Threshold: %f"), RotationThreshold);
	UE_LOG(LogTemp, Warning, TEXT("Ship Rotation is Safe: %s"), shipUpAlignment >= RotationThreshold ? TEXT("True") : TEXT("False"));

	return shipUpAlignment >= RotationThreshold;
}

bool AShip::HasShipStoppedMoving()
{
	FVector LinearVelocity = ShipMesh->GetPhysicsLinearVelocity();
	FVector AngularVelocity = ShipMesh->GetPhysicsAngularVelocityInDegrees();

	return (LinearVelocity.Size() == 0 && AngularVelocity.Size() == 0);
}

void AShip::TriggerExplode(bool ShipExplodedFromCrash)
{
	ShipExploded(ShipExplodedFromCrash);
}

void AShip::ShipReady()
{
	ShipStatus = EShipStatus::Ready;
	UE_LOG(LogTemp, Warning, TEXT("On Launch Pad, READY!"));
}

void AShip::ShipLanded()
{
	ShipStatus = EShipStatus::Landed;
	UE_LOG(LogTemp, Warning, TEXT("SHIP HAS LANDED!!!!"));

	DisableShipControls();
	bIsThrusting = false;

	OnShipLanded.Broadcast();
}

void AShip::ShipCrashed()
{
	ShipStatus = EShipStatus::Crashed;
	UE_LOG(LogTemp, Warning, TEXT("SHIP HAS CRASHED!!!!"));

	DisableShipControls();
	bIsThrusting = false;

	// Create explosion
	SpawnNiagaraSystem(NS_ExplosionSmall);
	
	// Audio
	if (SC_ExplosionSmall)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SC_ExplosionSmall, GetActorLocation());
	}
	
	// Smoke trail effect
	if (FireSmokeTrail)
	{
		FireSmokeTrail->Activate();
	}
}

void AShip::ShipExploded(bool ShipExplodedFromCrash)
{
	ShipStatus = EShipStatus::Exploded;
	UE_LOG(LogTemp, Error, TEXT("SHIP HAS EXPLODED!"));

	DisableShipControls();
	bIsThrusting = false;

	// Create explosion
	SpawnNiagaraSystem(NS_ExplosionBig);

	// Camera shake
	ShakeCamera();

	// Audio
	if (SC_ExplosionBig)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SC_ExplosionBig, GetActorLocation());
	}

	if (!ShipExplodedFromCrash)
	{
		// Freeze Ship
		ShipMesh->SetSimulatePhysics(false);
		ShipMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
		ShipMesh->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

		// Hide Ship mesh during explosion
		VisualMesh->SetVisibility(false, false);
		ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (FireSmokeTrail->IsVisible())
		{
			FireSmokeTrail->SetVisibility(false, false);
		}
	}

	OnShipDestroyed.Broadcast();
}

void AShip::DisableShipControls()
{
	if (LanderPlayerController)
	{
		DisableInput(LanderPlayerController);
	}
}

void AShip::SpawnNiagaraSystem(UNiagaraSystem* NiagaraSystemToSpawn, FVector LocationOffset)
{
	if (NiagaraSystemToSpawn)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraSystemToSpawn,
			GetActorLocation() + LocationOffset
		);
	}
}

void AShip::ShakeCamera()
{
	if (LanderPlayerController && ExplosionShake)
	{
		LanderPlayerController->ClientStartCameraShake(ExplosionShake);
	}
}
