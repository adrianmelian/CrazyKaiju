// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRHand.generated.h"

UCLASS()
class CRAZYKAIJU_API AVRHand : public APawn
{
	GENERATED_BODY()

public:
	AVRHand();

	//Motion Controllers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent* MotionControllerLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent* MotionControllerRight;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot = nullptr;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
