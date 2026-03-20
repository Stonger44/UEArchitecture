// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pad.h"
#include "NiagaraComponent.h"
#include "LandingPad.generated.h"

UCLASS()
class UEARCHITECTURE_API ALandingPad : public APad
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandingPad();

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* Fireworks;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void HandleShipLanded(APad* CurrentTouchdownPad);

	UPROPERTY(EditAnywhere, Category = "VFX")
	TArray<FVector> FireworkOffsets = {
		FVector(400.f, 0.f, 0.f),
		FVector(400.f, 200.f, 0.f),
		FVector(400.f, -200.f, 0.f)
	};

private:

};
