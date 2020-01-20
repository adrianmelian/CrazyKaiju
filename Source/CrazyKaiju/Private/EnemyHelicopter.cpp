// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

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
	
	auto PlayerLocation = PlayerPawn->GetActorLocation();
	auto TargetLocation = Target->GetComponentLocation();
	FVector Direction = PlayerLocation - TargetLocation;

	// Aim Destination at player
	AimAtPlayer(Direction);
	/*
	// Move Destination toward player, if distance is greater than X
	
	// Move Target toward destination
	

	
	float Throttle = Direction.Size() / 100;
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);

	auto ForceApplied = Direction * CurrentThrottle * MaxAcceleration;
	auto ForceLocation = Target->GetComponentLocation();
	Target->AddForceAtLocation(ForceApplied, ForceLocation);
	Target->AddForce(FVector(0, 0, 10000));

	UE_LOG(LogTemp, Warning, TEXT("Player Location: %s"), *PlayerLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Target Location: %s"), *TargetLocation.ToString());
	*/
}

// Called to bind functionality to input
void AEnemyHelicopter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyHelicopter::AimAtPlayer(FVector AimDirection)
{
	auto CurrentTargetRot = Target->GetForwardVector().Rotation();
	auto DestinationRot = AimDirection.Rotation();
	auto DeltaRot = DestinationRot - CurrentTargetRot;

	auto DeltaYaw = FMath::Clamp<float>(DeltaRot.Yaw, -1.f, 1.f);
	float YawRotateChange = (DeltaYaw * MaxDegreesPerSecond) * GetWorld()->DeltaTimeSeconds;
	float YawRotation = Target->RelativeRotation.Yaw + YawRotateChange;

	auto DeltaPitch = FMath::Clamp<float>(DeltaRot.Pitch, -1.f, 1.f);
	float PitchRotateChange = (DeltaPitch * MaxDegreesPerSecond) * GetWorld()->DeltaTimeSeconds;
	float PitchRotation = Target->RelativeRotation.Pitch + PitchRotateChange;

	Target->SetRelativeRotation(FRotator(PitchRotation, YawRotation, 0));


}

