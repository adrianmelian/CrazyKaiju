// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h" 
#include "Components/StaticMeshComponent.h" 
#include "Components/SkeletalMeshComponent.h" 
#include "DestructibleComponent.h"
#include "DestructibleMesh.h"
#include "PunchBagBuilding.generated.h"


UCLASS()
class CRAZYKAIJU_API APunchBagBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APunchBagBuilding();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Destructible")
	class UDestructibleComponent* DestructibleComponent2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible")
	class UDestructibleMesh* DestructibleMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	class UPhysicsConstraintComponent* PhysicsConstraint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	class UStaticMeshComponent* MovableMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	class USkeletalMeshComponent* MovableSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float RequiredHitStrength = 35000000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	int HitPoints = 3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float HitTimeDelay = 0.5f;

	// Event to Detect When an Actor Hit the building
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	float LastHitTime = 0.0f;

	int HitCount = 0;

	void SpawnDestructibleMesh();
	bool DestroyBuildingMesh();
};
