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

EFiringStatus UEnemyAimComponent::GetFiringStatus() const
{
	return FiringStatus;
}

void UEnemyAimComponent::AimAtPlayer()
{
	auto ThisWorld = GetWorld();
	auto PlayerPawn = ThisWorld->GetFirstPlayerController()->GetPawn();
	if (!ensure(Target && PlayerPawn)) { return; }

	auto DeltaTime = ThisWorld->DeltaTimeSeconds;
	auto PawnLocation = PlayerPawn->GetActorLocation();
	FRotator CurrentTargetRot = Target->GetComponentRotation();
	float TargetToPawnDot = FVector::DotProduct(CurrentTargetRot.Vector(), PlayerPawn->GetActorRotation().Vector());
	
	// Aim at player
	FRotator DeltaRot = UKismetMathLibrary::FindLookAtRotation(Target->GetComponentLocation(), PawnLocation);
	auto DesiredRot = UKismetMathLibrary::RInterpTo(CurrentTargetRot, DeltaRot, DeltaTime, RotateSpeed);
	Target->SetWorldRotation(FRotator(0, DesiredRot.Yaw, 0));

	// Determine Firing Status
	if (ThisWorld->GetTimeSeconds() - LastFireTime < ReloadTime)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (FMath::Abs(TargetToPawnDot) < 0.85)
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else
	{
		ShootAtPlayer();
	}

}

void UEnemyAimComponent::ShootAtPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Shooting!"));
	LastFireTime = GetWorld()->GetTimeSeconds();
}
