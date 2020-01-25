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

	auto DeltaTime = ThisWorld->DeltaTimeSeconds;
	auto PlayerLocation = PlayerPawn->GetActorLocation();
	auto TargetLocation = Target->GetComponentLocation();
	auto DestinationDirection = PlayerLocation - TargetLocation;

	// Move Destination toward player, if distance is greater than X
	auto DistanceToPlayer = FVector(DestinationDirection.X, DestinationDirection.Y, 0).Size();
	MovementSpeed = FMath::Clamp<float>(MovementSpeed + DistanceToPlayer, 0.f, MaxSpeed);
	auto TargetForward = Target->GetComponentRotation().Vector();
	if (Hover && DistanceToPlayer < HoverDistance) { TargetForward = -TargetForward/3; }
	Target->AddWorldOffset((TargetForward * MovementSpeed) * DeltaTime);
}

void UEnemyFlyingComponent::Initialize(UStaticMeshComponent* TargetToSet)
{
	Target = TargetToSet;
}

