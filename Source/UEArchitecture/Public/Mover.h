// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEARCHITECTURE_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mover")
	FVector LocalPositionA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mover")
	FVector LocalPositionB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mover")
	float MoveSpeed = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mover")
	bool bIsMovingToB;

private:	
	FVector WorldPositionA;
	FVector WorldPositionB;
	
		
};
