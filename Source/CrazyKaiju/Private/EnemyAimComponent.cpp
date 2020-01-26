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

	if (IntermittentAiming)
	{
		if (PauseAiming) 
		{
			if (GetWorld()->GetTimeSeconds() - LastPauseTime > IntermittentAimingTimeout)
			{
				PauseAiming = false;
			}
			else
			{
				return;
			}
		}
	}

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

	float DeltaTime = ThisWorld->DeltaTimeSeconds;
	FVector PawnLocation = PlayerPawn->GetActorLocation();
	FRotator CurrentTargetRot = Target->GetComponentRotation();
	FVector TargetForward = Target->GetForwardVector();
	FVector DestinationDirection = PlayerPawn->GetActorLocation() - Target->GetComponentLocation();
	float TargetToPawnDot = FVector::DotProduct(FVector(TargetForward.X, TargetForward.Y, 0), FVector(DestinationDirection.X, DestinationDirection.Y, 0).GetSafeNormal());
	
	// Aim at player
	FRotator DeltaRot = UKismetMathLibrary::FindLookAtRotation(Target->GetComponentLocation(), PawnLocation);
	FRotator DesiredRot = UKismetMathLibrary::RInterpTo(CurrentTargetRot, DeltaRot, DeltaTime, RotateSpeed);
	// Banking
	auto TargetRightDot = FVector::DotProduct(Target->GetRightVector(), DestinationDirection.GetSafeNormal());
	auto DesiredRoll = (TargetRightDot * 30);
	Target->SetWorldRotation(FRotator(0, DesiredRot.Yaw, DesiredRoll));

	// Determine Firing Status
	if (ThisWorld->GetTimeSeconds() - LastFireTime < ReloadTime)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (TargetToPawnDot < 0.7f)
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else
	{
		ShootAtPlayer();
	}

	// Intermittent Aiming
	if (IntermittentAiming)
	{
		float DestinationDistance = FVector(DestinationDirection.X, DestinationDirection.Y, 0).Size();
		
		if (DestinationDistance < IntermittentAimingDistance)
		{
			PauseAiming = true;
			LastPauseTime = ThisWorld->GetTimeSeconds();
		}
	}

}

void UEnemyAimComponent::ShootAtPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Shooting!"));
	LastFireTime = GetWorld()->GetTimeSeconds();
}
