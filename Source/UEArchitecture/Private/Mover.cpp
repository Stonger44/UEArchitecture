// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MovementSpeed = 1000;
	bIsMovingToB = true;
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		USceneComponent* OwnerRoot = Owner->GetRootComponent();
		OwnerRoot->SetMobility(EComponentMobility::Movable);

		WorldPositionA = Owner->GetTransform().TransformPosition(LocalPositionA);
		WorldPositionB = Owner->GetTransform().TransformPosition(LocalPositionB);
	}
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()) return;

	Move(DeltaTime);
}

void UMover::Move(float DeltaTime)
{
	if (!GetOwner()) return;

	FVector CurrentPosition = GetOwner()->GetActorLocation();
	FVector TargetPosition = bIsMovingToB ? WorldPositionB : WorldPositionA;

	FVector Direction = (TargetPosition - CurrentPosition).GetSafeNormal();
	FVector NewPosition = CurrentPosition + (MovementSpeed * DeltaTime * Direction);

	if (FVector::Dist(NewPosition, TargetPosition) < FVector::Dist(CurrentPosition, TargetPosition))
	{
		GetOwner()->SetActorLocation(NewPosition);
	}
	else
	{
		GetOwner()->SetActorLocation(TargetPosition);
		bIsMovingToB = !bIsMovingToB;
	}
}

