// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Engine/StaticMesh.h"
#include "Curves/CurveFloat.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PostProcessComponent.h"
#include "XRMotionControllerBase.h"
#include "MotionControllerComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "NavigationSystem.h"
//#include "DestructibleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "VRCharacter.generated.h"

UCLASS()
class CRAZYKAIJU_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent* MotionControllerLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent* MotionControllerRight;

	// IK
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm IK")
	class USkeletalMeshComponent* KaijuArmsRig = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm IK")
	class USceneComponent* KaijuArmsPV_Left = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm IK")
	class USceneComponent* KaijuArmsPV_Right = nullptr;

	UFUNCTION(BlueprintCallable)
	void MoveIK(UMotionControllerComponent* MotionController, UStaticMeshComponent* IKTarget);
	float CurrentThrottle = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Arm IK")
	float MaxAcceleration = 10000000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = "Arm IK")
	class UStaticMeshComponent* IKTarget_Left = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm IK")
	class UStaticMeshComponent* IKTarget_Right = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Arm IK")
	float DampingAmount = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm IK")
	class UChildActorComponent* SimHand_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm IK")
	class UChildActorComponent* SimHand_Right;

private:
	// Movement
	void MoveForward(float Throttle);
	void MoveRight(float Throttle);
	void RightTurn();
	void LeftTurn();
	void CorrectCameraOffset();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MovementSpeedMultiplier = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 TurnStep = 15;

	// Vignette
	void StartFade(float start, float end);
	void UpdateVignette();
	FVector2D GetMovementCenter();

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* VignetteMaterialBase = nullptr;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* VignetteVelocityRadius = nullptr;

	// Teleport
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TeleportTime = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup") // TODO REMOVE
	float MaxTeleportDistance = 500;

	UPROPERTY(EditDefaultsOnly, Category = "Setup") // TODO REMOVE
	float TeleportSimulationTime = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxTeleportSpeed = 750;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TeleportProjectileRadius = 10;

	// Components
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot = nullptr;

	UPROPERTY(VisibleAnywhere) // Visible anywhere because you dont want to edit the pointer (*)
	class UStaticMeshComponent* DestinationMarker = nullptr;

	UPROPERTY()
	class UPostProcessComponent* PostProcessComponent = nullptr;

	UPROPERTY()
	class UMaterialInstanceDynamic* VignetteInstanceDynamic = nullptr;

	// Health
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHeathPercent();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth = StartHealth;

	class USphereComponent* SimHandCollision_Left = nullptr;
};
