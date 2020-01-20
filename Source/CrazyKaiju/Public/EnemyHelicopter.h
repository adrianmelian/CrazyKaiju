// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyHelicopter.generated.h"

UCLASS()
class CRAZYKAIJU_API AEnemyHelicopter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyHelicopter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	class USceneComponent* HeliRoot;

	float CurrentThrottle = 0.f;

	void AimAtPlayer(FVector AimDirection);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float MaxDegreesPerSecond = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float MaxAcceleration = 40000000.f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* Target = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* HelicopterMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UPhysicsConstraintComponent* PhysicsConstraint = nullptr;

};
