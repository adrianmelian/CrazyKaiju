// Fill out your copyright notice in the Description page of Project Settings.

//#include "Engine/World.h"
#include "VRCharacter.h"
#include "Engine/StaticMesh.h"
#include "Curves/CurveFloat.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
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

// Sets default values
AVRCharacter::AVRCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root for movement
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VR Root"));
	VRRoot->SetupAttachment(GetRootComponent());

	// VR Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	// Motion Controllers
	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller - Left"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller - Right"));
	MotionControllerLeft->SetupAttachment(VRRoot);
	MotionControllerRight->SetupAttachment(VRRoot);
	MotionControllerLeft->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	MotionControllerRight->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	//MotionControllerLeft->SetVisibility(false);
	//MotionControllerRight->SetVisibility(false);

	// Destination Marker
	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destination Mesh"));
	DestinationMarker->SetupAttachment(VRRoot);

	// Post Processing Component
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Post-Process Component"));
	PostProcessComponent->SetupAttachment(VRRoot);
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	DestinationMarker->SetVisibility(false);

	if (VignetteMaterialBase == nullptr) return;
	VignetteInstanceDynamic = UMaterialInstanceDynamic::Create(VignetteMaterialBase, this);
	PostProcessComponent->AddOrUpdateBlendable(VignetteInstanceDynamic);
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CorrectCameraOffset();
	UpdateVignette();
	UpdateForearmTwist();
}


void AVRCharacter::StartFade(float start, float end)
{
	// Fade
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->PlayerCameraManager->StartCameraFade(start, end, 0.2f, FLinearColor::Black, false, true);
	}
}

void AVRCharacter::UpdateVignette()
{
	if (VignetteVelocityRadius == nullptr) return;
	float Speed = GetVelocity().Size();
	float Radius = VignetteVelocityRadius->GetFloatValue(Speed);
	VignetteInstanceDynamic->SetScalarParameterValue(FName("VignetteRadius"), Radius);

	FVector2D MovementCenter = GetMovementCenter();
	VignetteInstanceDynamic->SetVectorParameterValue(FName("VignetteCenter"), FLinearColor(MovementCenter.X, MovementCenter.Y, 0));
}

void AVRCharacter::UpdateForearmTwist()
{
}

FVector2D AVRCharacter::GetMovementCenter()
{
	FVector MovementDirection = GetVelocity().GetSafeNormal();
	if (MovementDirection.IsNearlyZero()) return FVector2D(0.5, 0.5);

	FVector WorldStationaryLocation;
	if (FVector::DotProduct(Camera->GetForwardVector(), MovementDirection) > 0)
	{
		WorldStationaryLocation = Camera->GetComponentLocation() + MovementDirection * 1000;
	}
	else
	{
		WorldStationaryLocation = Camera->GetComponentLocation() - MovementDirection * 1000;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr) return FVector2D(0.5, 0.5);

	FVector2D ScreenStationaryLocation;
	PlayerController->ProjectWorldLocationToScreen(WorldStationaryLocation, ScreenStationaryLocation);

	int32 SizeX, SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);
	ScreenStationaryLocation.X /= SizeX;
	ScreenStationaryLocation.Y /= SizeY;

	return ScreenStationaryLocation;
}

void AVRCharacter::CorrectCameraOffset()
{
	FVector CameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	CameraOffset.Z = 0;
	AddActorWorldOffset(CameraOffset);
	VRRoot->AddWorldOffset(-CameraOffset);
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AVRCharacter::MoveRight);
	PlayerInputComponent->BindAction(TEXT("RightTurn"), IE_Released, this, &AVRCharacter::RightTurn);
	PlayerInputComponent->BindAction(TEXT("LeftTurn"), IE_Released, this, &AVRCharacter::LeftTurn);

}

void AVRCharacter::MoveForward(float Throttle)
{
	AddMovementInput((Throttle * MovementSpeedMultiplier) * Camera->GetForwardVector());
}

void AVRCharacter::MoveRight(float Throttle)
{
	AddMovementInput((Throttle * MovementSpeedMultiplier) * Camera->GetRightVector());
}

void AVRCharacter::RightTurn()
{
	VRRoot->AddWorldRotation(FRotator(0, 30, 0));
}

void AVRCharacter::LeftTurn()
{
	VRRoot->AddWorldRotation(FRotator(0, -30, 0));
}
