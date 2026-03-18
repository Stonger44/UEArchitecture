// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "Pad.generated.h"

class AShip;

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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	AShip* Ship = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Graphics")
	UStaticMeshComponent* Pad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Graphics")
	UStaticMeshComponent* PadLights;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	UPointLightComponent* PadLight0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	UPointLightComponent* PadLight1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	UPointLightComponent* PadLight2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	UPointLightComponent* PadLight3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	UPointLightComponent* PadLight4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	FLinearColor White = FLinearColor(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	FLinearColor Red = FLinearColor(1.0f, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pad | Lights")
	FLinearColor Green = FLinearColor(0, 1.0f, 0);

	UPROPERTY()
	TArray<UPointLightComponent*> PadLightArray;

	UPROPERTY()
	bool bIsLightOn = false;

	FTimerHandle LightBlinkTimer;

	UFUNCTION()
	void StartBlinkingLights();

	UFUNCTION()
	void StopBlinkingLights();

	UFUNCTION()
	void BlinkLights();

	UFUNCTION()
	virtual void HandleShipReady(APad* CurrentTouchdownPad);
	
	UFUNCTION()
	virtual void HandleShipLaunched(APad* CurrentTouchdownPad);

	UFUNCTION()
	virtual void HandleShipLandingEvaluation(APad* CurrentTouchdownPad);

	UFUNCTION()
	virtual void HandleShipLanded(APad* CurrentTouchdownPad);

	UFUNCTION()
	virtual void HandleShipCrashedIntoPad(APad* CurrentCrashedIntoPad);

private:	

};
