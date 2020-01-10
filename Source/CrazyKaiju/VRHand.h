// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRHand.generated.h"

UCLASS()
class CRAZYKAIJU_API AVRHand : public APawn
{
	GENERATED_BODY()

public:
	AVRHand();

	USceneComponent* HandRoot;

	// Sets throttle between -1 and +1
	UFUNCTION(BlueprintCallable)
	void SetThrottle();

	// Max Force to Track in Newtons
	UPROPERTY(EditDefaultsOnly, Category = "Driving")
	float MaxAcceleration = 40000000.f;

	void MoveTarget();

	float CurrentThrottle = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USceneComponent* Destination = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* Target = nullptr;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

