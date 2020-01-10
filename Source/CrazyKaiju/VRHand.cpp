// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHand.h"
#include "Components/StaticMeshComponent.h"
#include "MotionControllerComponent.h"

// Sets default values
AVRHand::AVRHand()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HandRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HandRoot"));
	HandRoot->SetupAttachment(GetRootComponent());
	Destination = CreateDefaultSubobject<USceneComponent>(TEXT("Destination"));
	Destination->SetupAttachment(HandRoot);
	Target = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	Target->SetupAttachment(HandRoot);

}

void AVRHand::SetThrottle()
{
	FVector Destination_Loc = Destination->GetComponentLocation();
	FVector Target_Loc = Target->GetComponentLocation();
	FVector Direction = Destination_Loc - Target_Loc;
	float Throttle = Direction.Size() / 100;
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);

	UE_LOG(LogTemp, Warning, TEXT("Target_Velocity = %f"), CurrentThrottle);

	auto ForceApplied = Direction * CurrentThrottle * MaxAcceleration;
	auto ForceLocation = Target->GetComponentLocation();
	Target->AddForceAtLocation(ForceApplied, ForceLocation);
}

void AVRHand::MoveTarget()
{
	auto ForceApplied = Target->GetForwardVector() * CurrentThrottle * MaxAcceleration;
	auto ForceLocation = Target->GetComponentLocation();
	//auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	Target->AddForceAtLocation(ForceApplied, ForceLocation);
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

	SetThrottle();
	//MoveTarget();

}

// Called to bind functionality to input
void AVRHand::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

