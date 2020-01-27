// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"

#include "HomingMissile.h"
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

	// Randomize reload time a little
	ReloadTime = FMath::RandRange((ReloadTime * 0.75f), (ReloadTime * 1.25f));
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

void UEnemyAimComponent::Initialize(UStaticMeshComponent* TargetToSet, USceneComponent* MissileStartLocationToSet)
{
	Target = TargetToSet;
	MissileStartLocation = MissileStartLocationToSet;
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
	float DestinationDistance = FVector(DestinationDirection.X, DestinationDirection.Y, 0).Size();

	// Intermittent Aiming
	if (IntermittentAiming)
	{
		if (DestinationDistance < IntermittentAimingDistance)
		{
			PauseAiming = true;
			LastPauseTime = ThisWorld->GetTimeSeconds();
		}
	}

	// Determine Firing Status
	if (ThisWorld->GetTimeSeconds() - LastFireTime < ReloadTime)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (TargetToPawnDot < 0.7f)
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else if (DestinationDistance > ShootingMinDistance && DestinationDistance < ShootingMaxDistance)
	{
		ShootAtPlayer();
	}
}

void UEnemyAimComponent::ShootAtPlayer()
{
	if (!ensure(HomingMissile_BP)) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Shooting!"));
	GetWorld()->SpawnActor<AHomingMissile>(HomingMissile_BP, MissileStartLocation->GetComponentLocation(), MissileStartLocation->GetComponentRotation());

	LastFireTime = GetWorld()->GetTimeSeconds();
	// Randomize reload time a little
	ReloadTime = FMath::RandRange((ReloadTime * 0.75f), (ReloadTime * 1.25f));
}
