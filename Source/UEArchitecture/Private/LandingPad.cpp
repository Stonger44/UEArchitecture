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

	if (bAreFireworksBangingOff)
	{
		if (!GetWorldTimerManager().IsTimerActive(FireworkBangTimer))
		{
			float NextBangDelay = FMath::RandRange(0.1f, 0.2f);
			GetWorldTimerManager().SetTimer(
				FireworkBangTimer,
				this,
				&ALandingPad::PlayFireworkBangLoopAudio,
				NextBangDelay,
				false
			);
		}
	}
}

void ALandingPad::HandleShipLanded(APad* CurrentTouchdownPad)
{
	Super::HandleShipLanded(CurrentTouchdownPad);

	if (Fireworks && DifferentFireworks)
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
			}
		}
	}

	if (SC_FireworkWhistle)
	{
		GetWorldTimerManager().SetTimer(
			FireworkWhistleTimer,
			this,
			&ALandingPad::TriggerFireworkWhistleAudio,
			1.0f,
			false
		);
	}

	if (SC_FireworkBang)
	{
		GetWorldTimerManager().SetTimer(
			FireworkBangTimer,
			this,
			&ALandingPad::TriggerFireworkBangAudio,
			2.5f,
			false
		);
	}
}

void ALandingPad::TriggerFireworkWhistleAudio()
{
	if (SC_FireworkWhistle)
	{
		for (const FVector& Location : FireworkOffsets)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				SC_FireworkWhistle,
				GetActorLocation() + Location
			);
		}
	}
}

void ALandingPad::TriggerFireworkBangAudio()
{
	if (SC_FireworkBang)
	{
		for (const FVector& Location : FireworkOffsets)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				SC_FireworkBang,
				GetActorLocation() + Location
			);
		}

		bAreFireworksBangingOff = true;
	}
}

void ALandingPad::PlayFireworkBangLoopAudio()
{
	UGameplayStatics::PlaySoundAtLocation(
		this,
		SC_FireworkBang,
		GetActorLocation()
	);
}
