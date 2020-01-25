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

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float RotateSpeed = 1.f; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ReloadTime = 10.f;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UStaticMeshComponent* TargetToSet);
		
private:
	UStaticMeshComponent* Target = nullptr;
};
