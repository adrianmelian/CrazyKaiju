// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyPlane.generated.h"

UCLASS()
class CRAZYKAIJU_API AEnemyPlane : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	class USceneComponent* PlaneRoot;

	float CurrentThrottle = 0.f;

	void MoveTowardPlayer();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float MaxDegreesPerSecond = 45.0f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* PlaneMesh = nullptr;

private:
	float MovementMaxSpeed = 250.f;

	float DistanceToPlayer = 0.f;

	FVector DestinationDirection;

};
