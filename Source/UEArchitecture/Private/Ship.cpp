// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	SetRootComponent(ShipMesh);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(ShipMesh);
	CameraBoom->TargetArmLength = 1800;

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

void AShip::Thrust(const FInputActionValue& inputValue)
{
	if (bool currentValue = inputValue.Get<bool>())
	{
		const FVector thrust = GetActorUpVector() * ThrustStrength;
		// UE_LOG(LogTemp, Warning, TEXT("GetActorUpVector Impulse: %s"), *thrust.ToString());

		ShipMesh->AddForce(thrust, NAME_None, true);

		// DrawDebugSphere(GetWorld(), ShipMesh->GetCenterOfMass(), 10, 16, FColor::Green, false, -1, 1, .5);
	}
}

void AShip::Rotate(const FInputActionValue& inputValue)
{
	if (float currentValue = inputValue.Get<float>())
	{
		// UE_LOG(LogTemp, Warning, TEXT("Input Value: %f"), currentValue)
		const FVector torque = FVector(-currentValue, 0, 0) * TorqueStrength;

		ShipMesh->AddTorqueInRadians(torque, NAME_None, true);

	}
}
