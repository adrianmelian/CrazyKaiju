// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingMissile.generated.h"

UCLASS()
class CRAZYKAIJU_API AHomingMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomingMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Static Mesh Component for Missile Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* MissileMesh;

	// Projectile Movement Component for Missile Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	// Collision Component for Missile Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* MissileRoot;

	// Missile Properties
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MissileSpeed = 750.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MissileDamage = 10;

	// Particle Systems
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UNiagaraComponent* LaunchBlastParticles = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UNiagaraComponent* ImpactBlastParticles = nullptr;

	// Explosion Force
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class URadialForceComponent* ExplosionForce = nullptr;

	//class UNiagaraComponent* 

private:
	// Event to Detect When an Actor Overlaps the Missile Class
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void FlyAtPlayer();
};
