// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Camera/CameraComponent.h"

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

// Called when the game starts or when spawned
void AEnemyHelicopter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyHelicopter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerPawn)) { return; }
	
	auto PlayerLocation = PlayerPawn->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	auto TargetLocation = Target->GetComponentLocation();
	DestinationDirection = PlayerLocation - TargetLocation;

	// Aim Destination at player
	AimAtPlayer();


	// Move Destination toward player, if distance is greater than X
	float DistanceToPlayer = FVector(DestinationDirection.X, DestinationDirection.Y, 0).Size();
	
	if (DistanceToPlayer > 150.f)
	{
		MoveForward();
	}
	else if (DistanceToPlayer < 150.f)
	{
		MoveBackward();
	}

}

// Called to bind functionality to input
void AEnemyHelicopter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyHelicopter::AimAtPlayer()
{
	auto DestinationRot = DestinationDirection.Rotation();
	auto CurrentTargetRot = Target->GetForwardVector().Rotation();
	auto DeltaRot = DestinationRot - CurrentTargetRot;

	MovementSpeed = 1.f - (DestinationRot.Vector() - CurrentTargetRot.Vector()).Size();

	auto DeltaYaw = FMath::Clamp<float>(DeltaRot.Yaw, -1.f, 1.f);
	float DeltaRotate = (DeltaYaw * MaxDegreesPerSecond) * GetWorld()->DeltaTimeSeconds;
	Target->AddWorldRotation(FRotator(0, DeltaRotate, 0));
}

void AEnemyHelicopter::MoveForward()
{
	MovementSpeed = MovementSpeed * 100.f;
	auto TargetForward = Target->GetComponentRotation().Vector();
	Target->AddWorldOffset((TargetForward * MovementSpeed) * GetWorld()->DeltaTimeSeconds);
}


void AEnemyHelicopter::MoveBackward()
{
	MovementSpeed = MovementSpeed * 100.f;
	auto TargetForward = Target->GetComponentRotation().Vector();
	Target->AddWorldOffset((-TargetForward * MovementSpeed)* GetWorld()->DeltaTimeSeconds);
}

void AEnemyHelicopter::MoveVertically()
{
	// Get height of head and target
	MovementSpeed = MovementSpeed * 0.25f;
	auto HeightOffset = FMath::Clamp<float>(DestinationDirection.Z, -1.f, 1.f);
	//Target->AddWorldOffset(FVector(0, 0, ((HeightOffset * MovementSpeed) * GetWorld()->DeltaTimeSeconds)));
}

