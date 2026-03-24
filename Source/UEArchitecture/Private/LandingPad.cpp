// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingPad.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
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

void ALandingPad::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

// Called every frame
void ALandingPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALandingPad::HandleShipLanded(APad* CurrentTouchdownPad)
{
	Super::HandleShipLanded(CurrentTouchdownPad);

	if (Fireworks)
	{
		if (!FireworkOffsets.IsEmpty())
		{
			for (const FVector& Location : FireworkOffsets)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					Fireworks,
					GetActorLocation() + Location
				);

				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					DifferentFireworks,
					GetActorLocation() + Location
				);

				if (SC_FireworkWhistle)
				{
					UGameplayStatics::PlaySoundAtLocation(this, SC_FireworkWhistle, GetActorLocation());
				}

				if (SC_FireworkExplosion)
				{
					UGameplayStatics::PlaySoundAtLocation(this, SC_FireworkExplosion, GetActorLocation());
				}
			}
		}
	}
}


