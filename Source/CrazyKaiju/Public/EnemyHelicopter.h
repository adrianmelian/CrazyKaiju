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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	class USceneComponent* HeliRoot;

	float CurrentThrottle = 0.f;

	void MoveAtPlayer();
	void MoveVertically();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float MaxDegreesPerSecond = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float MaxSpeed = 75.f;

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Target = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* HelicopterMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	class UPhysicsConstraintComponent* PhysicsConstraint = nullptr;

private:
	float MovementSpeed = 10.f;

	float DistanceToPlayer;
	
	FVector DestinationDirection;

};
