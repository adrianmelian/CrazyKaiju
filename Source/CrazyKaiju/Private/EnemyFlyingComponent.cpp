// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"

#include "EnemyFlyingComponent.h"

// Sets default values for this component's properties
UEnemyFlyingComponent::UEnemyFlyingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFlyingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyFlyingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveAtPlayer();
}

void UEnemyFlyingComponent::MoveAtPlayer()
{
	auto ThisWorld = GetWorld();
	auto PlayerPawn = ThisWorld->GetFirstPlayerController()->GetPawn();
	if (!ensure(Target && PlayerPawn)) { return; }

	// Get Locations and Rotations
	auto DeltaTime = ThisWorld->DeltaTimeSeconds;
	auto PlayerLocation = PlayerPawn->GetActorLocation();
	auto TargetLocation = Target->GetComponentLocation();
	auto DestinationDirection = PlayerLocation - TargetLocation;
	auto TargetForward = Target->GetForwardVector();
	
	// Speed up movement if behind player
	float TargetToPawnDot = FVector::DotProduct(FVector(TargetForward.X, TargetForward.Y, 0), FVector(DestinationDirection.X, DestinationDirection.Y, 0).GetSafeNormal());
	/*
	if (TargetToPawnDot < 0.f) 
	{ 
		MovementSpeed = MaxSpeed * (1 + FMath::Abs(TargetToPawnDot)); 
	}
	else
	{
		MovementSpeed = MaxSpeed * (2 - FMath::Abs(TargetToPawnDot));
	}
	*/

	MovementSpeed = MaxSpeed;

	// If Hovering
	if (Hover)
	{
		float DistanceToPlayer = FVector(DestinationDirection.X, DestinationDirection.Y, 0).Size();
		if(DistanceToPlayer < HoverDistance) 
		{ 
			TargetForward = -TargetForward * 0.5f; // Move at half speed backwards 
		}
	}
	
	// Move
	Target->AddWorldOffset((TargetForward * MovementSpeed) * DeltaTime);
}

void UEnemyFlyingComponent::Initialize(UStaticMeshComponent* TargetToSet)
{
	Target = TargetToSet;
}

