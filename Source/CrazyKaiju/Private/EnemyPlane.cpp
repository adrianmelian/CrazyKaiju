// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	auto DeltaTime = ThisWorld->DeltaTimeSeconds;
	auto PawnLocation = PlayerPawn->GetActorLocation();
	auto DestinationDistance = PawnLocation - PlaneMesh->GetComponentLocation();
	FRotator CurrentPlaneRot = PlaneMesh->GetComponentRotation();
	FRotator DeltaRot = UKismetMathLibrary::FindLookAtRotation(PlaneMesh->GetComponentLocation(), PawnLocation);

	// Aim at player
	auto DesiredPlaneRot = UKismetMathLibrary::RInterpTo(CurrentPlaneRot, DeltaRot, DeltaTime, MaxRotationSpeed);
	auto PlaneToPlayerDot = FVector::DotProduct(CurrentPlaneRot.Vector(), DeltaRot.Vector());
	UE_LOG(LogTemp, Warning, TEXT("Dot = %f"), PlaneToPlayerDot);

	PlaneMesh->SetWorldRotation(FRotator(0, DesiredPlaneRot.Yaw, 0));

	// Move Forward
	float MovementSpeed = FMath::Clamp<float>(MovementMaxSpeed - (DestinationDistance.Size() / 4), MovementMaxSpeed / 2, MovementMaxSpeed);
	if (PlaneToPlayerDot > 0.f) { MovementSpeed = MovementSpeed * 2; }
	//UE_LOG(LogTemp, Warning, TEXT("MovementSpeed = %f"), MovementSpeed);
	FVector TargetForward = CurrentPlaneRot.Vector();
	PlaneMesh->AddWorldOffset((TargetForward * MovementSpeed) * DeltaTime);
}


