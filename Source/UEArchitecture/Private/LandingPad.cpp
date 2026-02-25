// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingPad.h"
#include "Ship.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALandingPad::ALandingPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALandingPad::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALandingPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


