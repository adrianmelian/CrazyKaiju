// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAimComponent.generated.h"

UENUM()
enum class EFiringStatus : uint8
{
	Reloading,
	Aiming,
	Ready,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAZYKAIJU_API UEnemyAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyAimComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Reloading;

public:	
	EFiringStatus GetFiringStatus() const;
	
	void AimAtPlayer();
	void ShootAtPlayer();

	double LastFireTime = 0;

	double LastPauseTime = 0;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UStaticMeshComponent* TargetToSet, USceneComponent* MissileStartLocationToSet);

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	bool IgnorePitch = true;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	bool LaunchProjectiles = true;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	bool IntermittentAiming = false;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float RotateSpeed = 1.f; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReloadTime = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ShootingMinDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ShootingMaxDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float IntermittentAimingTimeout = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float IntermittentAimingDistance = 150.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<class AHomingMissile> HomingMissile_BP = nullptr;
		
private:
	UStaticMeshComponent* Target = nullptr;

	USceneComponent* MissileStartLocation = nullptr;

	float MaxSpeed = 1.f;

	bool PauseAiming = false;
};
