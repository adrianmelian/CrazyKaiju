// Fill out your copyright notice in the Description page of Project Settings.

//#include "Engine/World.h"
#include "VRCharacter.h"

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

	// Destination Marker
	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destination Mesh"));
	DestinationMarker->SetupAttachment(VRRoot);

	// Post Processing Component
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Post-Process Component"));
	PostProcessComponent->SetupAttachment(VRRoot);

	// Motion Controllers
	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller - Left"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller - Right"));
	MotionControllerLeft->SetupAttachment(VRRoot);
	MotionControllerRight->SetupAttachment(VRRoot);
	MotionControllerLeft->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	MotionControllerRight->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	MotionControllerLeft->SetVisibility(false);
	MotionControllerRight->SetVisibility(false);

	//IK Arms
	KaijuArmsRig = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Kaiju Arms Rig"));
	KaijuArmsRig->SetupAttachment(Camera);
	
	KaijuArmsPV_Right = CreateDefaultSubobject<USceneComponent>(TEXT("KaijuArmsPV Right"));
	KaijuArmsPV_Left = CreateDefaultSubobject<USceneComponent>(TEXT("KaijuArmsPV Left"));
	KaijuArmsPV_Right->AttachToComponent(KaijuArmsRig, FAttachmentTransformRules::KeepRelativeTransform, FName("R_Wrist_Jnt"));
	KaijuArmsPV_Left->AttachToComponent(KaijuArmsRig, FAttachmentTransformRules::KeepRelativeTransform, FName("L_Wrist_Jnt"));

	// IK Arms Physics
	IKTarget_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IKTarget Left"));
	IKTarget_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IKTarget Right"));
	IKTarget_Left->SetupAttachment(VRRoot);
	IKTarget_Right->SetupAttachment(VRRoot);
	IKTarget_Left->SetVisibility(false);
	IKTarget_Right->SetVisibility(false);
	IKTarget_Left->SetSimulatePhysics(true);
	IKTarget_Right->SetSimulatePhysics(true);
	IKTarget_Left->SetEnableGravity(false);
	IKTarget_Right->SetEnableGravity(false);
	IKTarget_Left->SetLinearDamping(DampingAmount);
	IKTarget_Right->SetLinearDamping(DampingAmount);
	IKTarget_Left->SetAngularDamping(DampingAmount);
	IKTarget_Right->SetAngularDamping(DampingAmount);
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	DestinationMarker->SetVisibility(false);
	
	if (VignetteMaterialBase == nullptr) return;
	VignetteInstanceDynamic = UMaterialInstanceDynamic::Create(VignetteMaterialBase, this);
	PostProcessComponent->AddOrUpdateBlendable(VignetteInstanceDynamic);

	IKTarget_Left->OnComponentHit.AddDynamic(this, &AVRCharacter::OnHit);
	IKTarget_Right->OnComponentHit.AddDynamic(this, &AVRCharacter::OnHit);
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CorrectCameraOffset();
	UpdateVignette();

	MoveIK(MotionControllerLeft, IKTarget_Left);
	MoveIK(MotionControllerRight, IKTarget_Right);
}

// Punching
void AVRCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DamageDestructible(OtherComp, Hit.Location);
}

void AVRCharacter::DamageDestructible(UPrimitiveComponent* OtherComp, FVector HitLocation)
{
	UDestructibleComponent* Building = Cast<UDestructibleComponent>(OtherComp);
	if (Building == nullptr) { return; }

	Building->ApplyRadiusDamage(BaseDamage, HitLocation, DamageRadius, ImpulseStrength, false);
}

// Movement
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AVRCharacter::MoveRight);
	PlayerInputComponent->BindAction(TEXT("RightTurn"), IE_Released, this, &AVRCharacter::RightTurn);
	PlayerInputComponent->BindAction(TEXT("LeftTurn"), IE_Released, this, &AVRCharacter::LeftTurn);
	PlayerInputComponent->BindAction(TEXT("KaijuBeam"), IE_Pressed, this, &AVRCharacter::LaserAttack);

}
void AVRCharacter::MoveIK(UMotionControllerComponent* MotionController, UStaticMeshComponent* IKTarget)
{
	FVector Destination_Loc = MotionController->GetComponentLocation();
	FVector Target_Loc = IKTarget->GetComponentLocation();
	FVector Direction = Destination_Loc - Target_Loc;
	float Throttle = Direction.Size() / 500.f;
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, 0, 1);

	//UE_LOG(LogTemp, Warning, TEXT("Target_Velocity = %f"), CurrentThrottle);

	auto ForceApplied = Direction * CurrentThrottle * MaxAcceleration;
	auto ForceLocation = IKTarget->GetComponentLocation();
	IKTarget->AddForceAtLocation(ForceApplied, ForceLocation);
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

void AVRCharacter::LaserAttack()
{
	FHitResult HitResult;
	FVector StartLocation = Camera->GetComponentLocation();
	FVector LookDirection = Camera->GetForwardVector();
	FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility))
	{
		auto OtherComp = HitResult.GetComponent();
		DamageDestructible(OtherComp, HitResult.Location);
	}
}

void AVRCharacter::StartFade(float start, float end)
{
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

// Health & Damage
float AVRCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamageInt = FMath::RoundToInt(Damage);
	int32 DamageToApply = FMath::Clamp<int>(DamageInt, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health: %i"), CurrentHealth);

	return 0.0f;
}

float AVRCharacter::GetHeathPercent()
{
	return (float)CurrentHealth / (float)StartHealth;
}
