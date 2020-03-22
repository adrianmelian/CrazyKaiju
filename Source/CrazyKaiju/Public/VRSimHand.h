// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "DestructibleComponent.h"

#include "VRSimHand.generated.h"

UCLASS()
class CRAZYKAIJU_API AVRSimHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRSimHand();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	class USphereComponent* Collision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setup")
	class UStaticMeshComponent* DebugMesh = nullptr;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float BaseDamage = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DamageRadius = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ImpulseStrength = 100000.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
