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
	float DestinationDistance = DestinationDirection.Size();
	auto TargetForward = Target->GetForwardVector();

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
	MovementSpeed = MaxSpeed + (DestinationDistance * 0.1f);
	Target->AddWorldOffset((TargetForward * MovementSpeed) * DeltaTime);
}

void UEnemyFlyingComponent::Initialize(UStaticMeshComponent* TargetToSet)
{
	Target = TargetToSet;
}

