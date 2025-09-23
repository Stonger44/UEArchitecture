// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "LandingPad.h"
#include "LaunchPad.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	SetRootComponent(ShipMesh);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(ShipMesh);
	CameraBoom->TargetArmLength = 2000;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ShipMesh->SetAngularDamping(5);

}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* enhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			enhancedInputLocalPlayerSubsystem->AddMappingContext(ShipMappingContext, 0);
		}
	}

	// Calculate Landing Rotation Threshold
	LandingRotationThreshold = FMath::Cos(FMath::DegreesToRadians(MaxLandingRotation));
	UE_LOG(LogTemp, Warning, TEXT("Landing Rotation Threshold: %f"), LandingRotationThreshold);

	ShipStatus = EShipStatus::Ready;
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Actions to Functions
		EnhancedInputComponent->BindAction(ThrustAction, ETriggerEvent::Triggered, this, &AShip::Thrust);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AShip::Rotate);
	}

}

void AShip::Thrust(const FInputActionValue& InputValue)
{
	if (bool currentValue = InputValue.Get<bool>())
	{
		const FVector thrust = GetActorUpVector() * ThrustStrength;
		// UE_LOG(LogTemp, Warning, TEXT("GetActorUpVector Impulse: %s"), *thrust.ToString());

		ShipMesh->AddForce(thrust, NAME_None, true);

		// DrawDebugSphere(GetWorld(), ShipMesh->GetCenterOfMass(), 10, 16, FColor::Green, false, -1, 1, .5);
		
		if (ShipStatus == EShipStatus::Ready)
		{
			ShipStatus = EShipStatus::Launched;
		}
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
			//else if (false)
			//{
			//	// Placeholder for fuel pickups
			//}
			else
			{
				OnShipCrashed();
			}
		}
	}
}

void AShip::Crash()
{
	OnShipCrashed();
}

void AShip::CheckShipLanding(AActor* Other)
{
	if (IsShipSpeedSafe() && IsShipRotationSafe())
	{
		if (ALaunchPad* LaunchPad = Cast<ALaunchPad>(Other))
		{
			OnShipReady();
		}
		else if (ALandingPad* LandingPad = Cast<ALandingPad>(Other))
		{
			OnShipLanded();
		}
	}
	else
	{
		OnShipCrashed();
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


void AShip::OnShipReady()
{
	ShipStatus = EShipStatus::Ready;
	UE_LOG(LogTemp, Warning, TEXT("On Launch Pad, Ready!"));
}

void AShip::OnShipLanded()
{
	ShipStatus = EShipStatus::Landed;
	UE_LOG(LogTemp, Warning, TEXT("LANDED!!!!"));
}

void AShip::OnShipCrashed()
{
	ShipStatus = EShipStatus::Crashed;
	UE_LOG(LogTemp, Warning, TEXT("CRAAAASH!!!!"));
	UE_LOG(LogTemp, Warning, TEXT("Restarting Level!"));

	// TODO: Disable Player Input
	// TODO: Create explosion, smoke/burning effect

	TriggerLevelRestart();
}

void AShip::TriggerLevelRestart()
{
	GetWorld()->GetTimerManager().SetTimer(LevelLoadTimer, this, &AShip::RestartLevel, 3.0f, false);
}

void AShip::RestartLevel()
{
	// Restart Level
	FName CurrentlevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, CurrentlevelName, false);
}