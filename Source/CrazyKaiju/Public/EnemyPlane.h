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

public:
	class USceneComponent* PlaneRoot;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float MaxRotationSpeed = 0.25f;

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* PlaneMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UEnemyAimComponent* AimingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UEnemyFlyingComponent* FlyingComponent = nullptr;
};
