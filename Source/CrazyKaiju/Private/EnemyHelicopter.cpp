// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Camera/CameraComponent.h"

#include "EnemyAimComponent.h"
#include "EnemyHelicopter.h"

// Sets default values
AEnemyHelicopter::AEnemyHelicopter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeliRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HandRoot"));
	HeliRoot->SetupAttachment(GetRootComponent());
	Target = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	Target->SetupAttachment(HeliRoot);
	HelicopterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelicopterMesh"));
	HelicopterMesh->SetupAttachment(HeliRoot);
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constaint"));
	PhysicsConstraint->SetupAttachment(RootComponent);

}

// Called every frame
void AEnemyHelicopter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MoveAtPlayer();
}

void AEnemyHelicopter::MoveAtPlayer()
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerPawn)) { return; }

	auto PlayerLocation = PlayerPawn->GetActorLocation();
	auto TargetLocation = Target->GetComponentLocation();
	DestinationDirection = PlayerLocation - TargetLocation;

	// Move Destination toward player, if distance is greater than X
	DistanceToPlayer = FVector(DestinationDirection.X, DestinationDirection.Y, 0).Size();
	MovementSpeed = FMath::Clamp<float>(MovementSpeed + DistanceToPlayer, 0.f, MaxSpeed);
	auto TargetForward = Target->GetComponentRotation().Vector();
	if (DistanceToPlayer < 150.f) { TargetForward = -TargetForward; }
	Target->AddWorldOffset((TargetForward * MovementSpeed) * GetWorld()->DeltaTimeSeconds);
}

void AEnemyHelicopter::MoveVertically()
{
	// Get height of head and target
	MovementSpeed = MovementSpeed * 0.25f;
	auto HeightOffset = FMath::Clamp<float>(DestinationDirection.Z, -1.f, 1.f);
	//Target->AddWorldOffset(FVector(0, 0, ((HeightOffset * MovementSpeed) * GetWorld()->DeltaTimeSeconds)));
}

