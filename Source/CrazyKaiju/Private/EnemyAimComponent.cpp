// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"

#include "EnemyAimComponent.h"

// Sets default values for this component's properties
UEnemyAimComponent::UEnemyAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyAimComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AimAtPlayer();
}

void UEnemyAimComponent::Initialize(UStaticMeshComponent* TargetToSet)
{
	Target = TargetToSet;
}

void UEnemyAimComponent::AimAtPlayer()
{
	auto ThisWorld = GetWorld();
	auto PlayerPawn = ThisWorld->GetFirstPlayerController()->GetPawn();
	if (!ensure(Target && PlayerPawn)) { return; }

	auto DeltaTime = ThisWorld->DeltaTimeSeconds;
	auto PawnLocation = PlayerPawn->GetActorLocation();
	FRotator CurrentPlaneRot = Target->GetComponentRotation();
	FRotator DeltaRot = UKismetMathLibrary::FindLookAtRotation(Target->GetComponentLocation(), PawnLocation);

	// Aim at player
	auto DesiredRot = UKismetMathLibrary::RInterpTo(CurrentPlaneRot, DeltaRot, DeltaTime, RotateSpeed);
	Target->SetWorldRotation(FRotator(0, DesiredRot.Yaw, 0));
}
