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

	// Move up and down to match player
	MoveVertically();

	// Move Destination toward player, if distance is greater than X
	float DistanceToPlayer = DestinationDirection.Size();
	UE_LOG(LogTemp, Warning, TEXT("Distance= %f"), DistanceToPlayer);
	if (DistanceToPlayer > 200.f)
	{
		MoveForward();
	}
}

// Called to bind functionality to input
void AEnemyHelicopter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyHelicopter::AimAtPlayer()
{
	auto DestinationRot = DestinationDirection.GetSafeNormal().Rotation();
	auto CurrentTargetRot = Target->GetForwardVector().Rotation();
	auto DeltaRot = DestinationRot - CurrentTargetRot;
	MovementSpeed = 1 - (DestinationRot.Vector() - CurrentTargetRot.Vector()).Size();

	auto DeltaYaw = FMath::Clamp<float>(DeltaRot.Yaw, -1.f, 1.f);
	float YawRotateChange = (DeltaYaw * MaxDegreesPerSecond) * GetWorld()->DeltaTimeSeconds;
	float YawRotation = Target->RelativeRotation.Yaw + YawRotateChange;

	auto DeltaPitch = FMath::Clamp<float>(DeltaRot.Pitch, -1.f, 1.f);
	float PitchRotateChange = (DeltaPitch * MaxDegreesPerSecond) * GetWorld()->DeltaTimeSeconds;
	float PitchRotation = Target->RelativeRotation.Pitch + PitchRotateChange;

	if (FMath::Abs(YawRotation) > 180) { YawRotation = -YawRotation; }
	Target->SetRelativeRotation(FRotator(PitchRotation, YawRotation, 0));
}

void AEnemyHelicopter::MoveForward()
{
	auto TargetForward = Target->GetForwardVector();
	Target->AddRelativeLocation(TargetForward * MovementSpeed);
}

void AEnemyHelicopter::MoveVertically()
{
	// Get height of head and target
	auto HeightOffset = FMath::Clamp<float>(DestinationDirection.Z, -1.f, 1.f);
	Target->AddRelativeLocation(FVector(0, 0, (HeightOffset * (MovementSpeed * 0.25f))));
}

