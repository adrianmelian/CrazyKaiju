// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "EnemyAimComponent.h"
#include "EnemyFlyingComponent.h"
#include "EnemyHelicopter.h"

// Sets default values
AEnemyHelicopter::AEnemyHelicopter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HeliRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HandRoot"));
	HeliRoot->SetupAttachment(GetRootComponent());
	Target = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	Target->SetupAttachment(HeliRoot);
	HelicopterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelicopterMesh"));
	HelicopterMesh->SetupAttachment(HeliRoot);
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constaint"));
	PhysicsConstraint->SetupAttachment(RootComponent);
	
	AimingComponent = CreateDefaultSubobject<UEnemyAimComponent>(TEXT("Aiming Component"));
	FlyingComponent = CreateDefaultSubobject<UEnemyFlyingComponent>(TEXT("Flying Component"));

}
