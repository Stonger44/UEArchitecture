// Fill out your copyright notice in the Description page of Project Settings.


/*-----TODO-----*\
-Landing
	-Speed Threshold
	-Rotation Threshold
	-Must land completely on pad
		-small walls with blinking lights on either side of pad, if ship hits these, game over
	-Celebration on successful landing

	-Physics Hit notification that ship has landed
\*-----TODO-----*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "LandingPad.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShipLanded);

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

	virtual void NotifyHit
	(
		class UPrimitiveComponent* MyComp,
		AActor* Other,
		class UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;

	FOnShipLanded OnShipLanded;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bShipHasLanded = false;

	UPROPERTY()
	UNiagaraComponent* CelebrationEffectComponent;
};
