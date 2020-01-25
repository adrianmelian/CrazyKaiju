// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaticMeshComponent.h"
#include "EnemyAimComponent.h"
#include "EnemyFlyingComponent.h"
#include "EnemyPlane.h"

// Sets default values
AEnemyPlane::AEnemyPlane()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PlaneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PlaneRoot"));
	PlaneRoot->SetupAttachment(GetRootComponent());
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelicopterMesh"));
	PlaneMesh->SetupAttachment(PlaneRoot);

	AimingComponent = CreateDefaultSubobject<UEnemyAimComponent>(TEXT("Aiming Component"));
	FlyingComponent = CreateDefaultSubobject<UEnemyFlyingComponent>(TEXT("Flying Component"));
}
