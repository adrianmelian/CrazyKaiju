// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"

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

	// Sets throttle between -1 and +1
	UFUNCTION(BlueprintCallable)
	void MoveIK(UMotionControllerComponent* MotionController, UStaticMeshComponent* IKTarget);
	float CurrentThrottle = 0.f;

	// Max Force to Track in Newtons
	UPROPERTY(EditDefaultsOnly, Category = "Arm IK")
	float MaxAcceleration = 10000000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = "Arm IK")
	class UStaticMeshComponent* IKTarget_Left = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm IK")
	class UStaticMeshComponent* IKTarget_Right = nullptr;

private:
	// Movement
	void MoveForward(float Throttle);
	void MoveRight(float Throttle);
	void RightTurn();
	void LeftTurn();
	void CorrectCameraOffset();

	// Vignette
	void StartFade(float start, float end);
	void UpdateVignette();

	FVector2D GetMovementCenter();

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* VignetteMaterialBase = nullptr;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* VignetteVelocityRadius = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MovementSpeedMultiplier = 0.2f;

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

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 TurnStep = 15;

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

};
