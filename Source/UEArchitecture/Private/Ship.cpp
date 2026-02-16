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

	FireSmokeTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireSmokeTrail"));
	FireSmokeTrail->SetupAttachment(ShipMesh);
	FireSmokeTrail->bAutoActivate = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ShipMesh->SetAngularDamping(5);

}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* enhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			enhancedInputLocalPlayerSubsystem->AddMappingContext(ShipMappingContext, 0);
		}
	}

	// Calculate Landing Rotation Threshold
	LandingRotationThreshold = FMath::Cos(FMath::DegreesToRadians(MaxLandingRotation));
	UE_LOG(LogTemp, Warning, TEXT("Landing Rotation Threshold: %f"), LandingRotationThreshold);

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

	// For Development
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, INDEFINITELY_LOOPING_DURATION, FColor::Green, FString::Printf(TEXT("Fuel: %.2f / %.2f"), Fuel, MaxFuel));
	}
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
	// UE_LOG(LogTemp, Warning, TEXT("IsThrusting: %s"), InputValue.Get<bool>() ? TEXT("true") : TEXT("false"))

	if (bIsThrusting && Fuel > 0)
	{
		if (ShipStatus == EShipStatus::Ready)
		{
			ShipStatus = EShipStatus::Launched;
		}

		const FVector thrust = GetActorUpVector() * ThrustStrength;
		// UE_LOG(LogTemp, Warning, TEXT("GetActorUpVector Impulse: %s"), *thrust.ToString());

		ShipMesh->AddForce(thrust, NAME_None, true);

		// DrawDebugSphere(GetWorld(), ShipMesh->GetCenterOfMass(), 10, 16, FColor::Green, false, -1, 1, .5);

	}
}

void AShip::Rotate(const FInputActionValue& InputValue)
{
	if (float currentValue = InputValue.Get<float>())
	{
		// UE_LOG(LogTemp, Warning, TEXT("Input Value: %f"), currentValue)
		const FVector torque = FVector(-currentValue, 0, 0) * TorqueStrength;

		ShipMesh->AddTorqueInRadians(torque, NAME_None, true);

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
				CheckShipLanding(Other);
			}
			else
			{
				ShipCrashed();
			}
		}
	}
}

void AShip::CheckShipLanding(AActor* Other)
{
	if (IsShipSpeedSafe() && IsShipRotationSafe())
	{
		if (ALaunchPad* LaunchPad = Cast<ALaunchPad>(Other))
		{
			ShipReady();
		}
		else if (ALandingPad* LandingPad = Cast<ALandingPad>(Other))
		{
			ShipLanded();
		}
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

bool AShip::IsShipRotationSafe()
{
	FVector shipUpVector = this->GetActorUpVector();

	// DotProduct for ship alignment
	float shipUpAlignment = FVector::DotProduct(shipUpVector, FVector::UpVector);
	UE_LOG(LogTemp, Warning, TEXT("Ship Up Alignment: %f"), shipUpAlignment);

	// Compare ship alignment with threshold
	UE_LOG(LogTemp, Warning, TEXT("Rotation Threshold: %f"), LandingRotationThreshold);
	UE_LOG(LogTemp, Warning, TEXT("Ship Rotation is Safe: %s"), shipUpAlignment >= LandingRotationThreshold ? TEXT("True") : TEXT("False"));

	return shipUpAlignment >= LandingRotationThreshold;
}

void AShip::TriggerCrash()
{
	ShipCrashed();
}

void AShip::TriggerExplode()
{
	ShipExploded();
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
}

void AShip::ShipCrashed()
{
	ShipStatus = EShipStatus::Crashed;
	UE_LOG(LogTemp, Warning, TEXT("SHIP HAS CRASHED!!!!"));

	DisableShipControls();
	bIsThrusting = false;

	// Create explosion
	SpawnExplosion(NS_ExplosionSmall);
	
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

	// fire on ground


	OnShipDestroyed.Broadcast();
}

void AShip::ShipExploded()
{
	ShipStatus = EShipStatus::Exploded;
	UE_LOG(LogTemp, Error, TEXT("SHIP HAS EXPLODED!"));

	DisableShipControls();
	bIsThrusting = false;

	// Create explosion
	SpawnExplosion(NS_ExplosionBig);

	// Camera shake
	ShakeCamera();

	// Audio
	if (SC_ExplosionBig)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SC_ExplosionBig, GetActorLocation());
	}

	// Freeze Ship
	ShipMesh->SetSimulatePhysics(false);
	ShipMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	ShipMesh->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	// Hide Ship mesh during explosion
	ShipMesh->SetVisibility(false, true);
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnShipDestroyed.Broadcast();
}

void AShip::DisableShipControls()
{
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}

void AShip::SpawnExplosion(UNiagaraSystem* ExplosionToSpawn)
{
	if (ExplosionToSpawn)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionToSpawn,
			GetActorLocation()
		);
	}
}

void AShip::ShakeCamera()
{
	if (PlayerController && ExplosionShake)
	{
		PlayerController->ClientStartCameraShake(ExplosionShake);
	}
}
