// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "Pad.generated.h"

UCLASS()
class UEARCHITECTURE_API APad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Graphics")
	UStaticMeshComponent* Pad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Graphics")
	UStaticMeshComponent* PadLights;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	UPointLightComponent* PadLight1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	UPointLightComponent* PadLight2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	UPointLightComponent* PadLight3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	UPointLightComponent* PadLight4;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
