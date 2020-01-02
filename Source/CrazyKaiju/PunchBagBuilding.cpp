// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchBagBuilding.h"

// Sets default values
APunchBagBuilding::APunchBagBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Movable Mesh"));
	MovableMesh->SetupAttachment(RootComponent);
	MovableMesh->SetSimulatePhysics(true);
	MovableMesh->SetCollisionProfileName("Enemy");
	MovableMesh->SetEnableGravity(false);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constaint"));
	PhysicsConstraint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APunchBagBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APunchBagBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

