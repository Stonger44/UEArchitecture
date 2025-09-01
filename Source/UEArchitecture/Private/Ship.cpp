// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "EnhancedInputComponent.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	SetRootComponent(ShipMesh);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(ShipMesh);
	CameraBoom->TargetArmLength = 1000.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Impulse Strength: %f"), ImpulseStrength);
	UE_LOG(LogTemp, Warning, TEXT("Torque Strength: %f"), TorqueStrength);
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
	}

}

void AShip::Thrust(const FInputActionValue& value)
{
	if (bool currentValue = value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Thrust triggered!"));
	}
}
