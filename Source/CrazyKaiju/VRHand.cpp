// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHand.h"
#include "MotionControllerComponent.h"

// Sets default values
AVRHand::AVRHand()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root for movement
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VR Root"));
	VRRoot->SetupAttachment(GetRootComponent());

	// Motion Controllers
	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller - Left"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller - Right"));
	MotionControllerLeft->SetupAttachment(VRRoot);
	MotionControllerRight->SetupAttachment(VRRoot);
	MotionControllerLeft->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	MotionControllerRight->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	MotionControllerLeft->SetVisibility(false);
	MotionControllerRight->SetVisibility(false);

}

// Called when the game starts or when spawned
void AVRHand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRHand::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

