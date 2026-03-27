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
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(FireworkBangTimer);
	GetWorldTimerManager().ClearTimer(FireworkWhistleTimer);
}

// Called every frame
void ALandingPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	for (const FVector& Location : FireworkOffsets)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SC_FireworkWhistle,
			GetActorLocation() + Location
		);
	}
}

void ALandingPad::TriggerFireworkBangAudio()
{
	if (bHaveInitialFireworksFired)
	{
		int32 RandomIndex = FMath::RandRange(0, FireworkOffsets.Num() - 1);

		UGameplayStatics::PlaySoundAtLocation(
			this,
			SC_FireworkBang,
			GetActorLocation() + FireworkOffsets[RandomIndex]
		);
	}
	else
	{
		for (const FVector& Location : FireworkOffsets)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				SC_FireworkBang,
				GetActorLocation() + Location
			);
		}

		bHaveInitialFireworksFired = true;
	}

	float NextBangDelay = FMath::RandRange(FireworkBangAudioDelayMin, FireworkBangAudioDelayMax);
	GetWorldTimerManager().SetTimer(
		FireworkBangTimer,
		this,
		&ALandingPad::TriggerFireworkBangAudio,
		NextBangDelay,
		false
	);
}
