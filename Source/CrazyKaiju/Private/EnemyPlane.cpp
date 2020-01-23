// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"

#include "VRCharacter.h"
#include "EnemyPlane.h"

// Sets default values
AEnemyPlane::AEnemyPlane()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlaneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PlaneRoot"));
	PlaneRoot->SetupAttachment(GetRootComponent());
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelicopterMesh"));
	PlaneMesh->SetupAttachment(PlaneRoot);

}

// Called when the game starts or when spawned
void AEnemyPlane::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveTowardPlayer();

}

// Called to bind functionality to input
void AEnemyPlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyPlane::MoveTowardPlayer()
{
	auto ThisWorld = GetWorld();
	auto PlayerPawn = ThisWorld->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerPawn)) { return; }

	auto PawnLocation = PlayerPawn->GetActorLocation();

	auto DestinationDistance = PawnLocation - PlaneMesh->GetComponentLocation();
	DestinationDirection = DestinationDistance.GetSafeNormal();
	FRotator DestinationRot = DestinationDirection.Rotation();
	FRotator CurrentTargetRot = PlaneMesh->GetComponentRotation();
	FRotator DeltaRot = DestinationRot - CurrentTargetRot;
	auto DeltaTime = ThisWorld->DeltaTimeSeconds;

	// Aim at player
	float DeltaYaw = FMath::Clamp<float>(DeltaRot.Yaw, -1.f, 1.f);
	if (FMath::Abs(DeltaYaw) > 180) { DeltaYaw = -DeltaYaw; }
	float DeltaRotate = (DeltaYaw * MaxDegreesPerSecond) * DeltaTime;
	PlaneMesh->AddWorldRotation(FRotator(0, DeltaRotate, 0));

	// Move Forward
	float MovementSpeed = FMath::Clamp<float>(MovementMaxSpeed - (DistanceToPlayer / 4), MovementMaxSpeed / 2, MovementMaxSpeed);
	FVector TargetForward = CurrentTargetRot.Vector();
	PlaneMesh->AddWorldOffset((TargetForward * MovementSpeed) * DeltaTime);
}


