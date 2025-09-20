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

	if (Other && Other != this)
	{
		if (!bShipHasLanded && Other->IsA(ALandingPad::StaticClass()))
		{
			CheckShipLanding();
		}

		// TODO: this condition only stops the restart initially; once the ship launches, if it hits the launchpad, it should trigger a crash
		if (!bShipHasCrashed && !(Other->IsA(ALaunchPad::StaticClass())))
		{
			bShipHasCrashed = true;
			UE_LOG(LogTemp, Warning, TEXT("CRAAAASH!!!!"));
			UE_LOG(LogTemp, Warning, TEXT("Restarting Level!"));

			TriggerLevelRestart();
		}
	}
}

void AShip::CheckShipLanding()
{
	bool bIsShipVelocitySafe = IsShipSpeedSafe();
	bool bIsShipRotationSafe = IsShipRotationSafe();

	if (bIsShipVelocitySafe && bIsShipRotationSafe)
	{
		bShipHasLanded = true;
		UE_LOG(LogTemp, Warning, TEXT("SAAAAFE!!!!"));
	}
}

bool AShip::IsShipSpeedSafe()
{
	FVector ShipVelocity = this->GetVelocity();

	float shipSpeed = ShipVelocity.Size();

	bool bIsShipSpeedSafe = shipSpeed <= MaxLandingSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Ship speed: %f"), shipSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Max Landing speed: %f"), MaxLandingSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Ship Speed is Safe: %s"), bIsShipSpeedSafe ? TEXT("True") : TEXT("False"));

	return bIsShipSpeedSafe;
}

bool AShip::IsShipRotationSafe()
{
	FVector shipUpVector = this->GetActorUpVector();

	// DotProduct for ship alignment
	float shipUpAlignment = FVector::DotProduct(shipUpVector, FVector::UpVector);
	UE_LOG(LogTemp, Warning, TEXT("Ship Up Alignment: %f"), shipUpAlignment);

	// Compare ship alignment with threshold
	bool bIsShipRotationSafe = shipUpAlignment >= LandingRotationThreshold;
	UE_LOG(LogTemp, Warning, TEXT("Rotation Threshold: %f"), LandingRotationThreshold);
	UE_LOG(LogTemp, Warning, TEXT("Ship Rotation is Safe: %s"), bIsShipRotationSafe ? TEXT("True") : TEXT("False"));

	return bIsShipRotationSafe;
}

void AShip::TriggerLevelRestart()
{
	GetWorld()->GetTimerManager().SetTimer(LevelLoadTimer, this, &AShip::RestartLevel, 2.0f, false);
}

void AShip::RestartLevel()
{
	// Restart Level
	FName CurrentlevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, CurrentlevelName, false);
}