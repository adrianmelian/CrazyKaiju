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
	class USceneComponent* HeliRoot;

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Target = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* HelicopterMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	class UPhysicsConstraintComponent* PhysicsConstraint = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UEnemyAimComponent* AimingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UEnemyFlyingComponent* FlyingComponent = nullptr;

};
