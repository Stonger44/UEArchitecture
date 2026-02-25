// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "LandingPad.generated.h"

UCLASS()
class UEARCHITECTURE_API ALandingPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandingPad();

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* CelebrationEffect;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};
