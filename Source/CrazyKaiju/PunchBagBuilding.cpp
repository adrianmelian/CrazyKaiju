// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchBagBuilding.h"
#include "DestructibleComponent.h"

// Sets default values
APunchBagBuilding::APunchBagBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Movable Mesh"));
	MovableMesh->SetupAttachment(RootComponent);
	MovableMesh->SetSimulatePhysics(true);
	MovableMesh->SetCollisionProfileName("Enemy");
	MovableMesh->SetEnableGravity(false);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constaint"));
	PhysicsConstraint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APunchBagBuilding::BeginPlay()
{
	Super::BeginPlay();
	
	MovableMesh->OnComponentHit.AddDynamic(this, &APunchBagBuilding::OnHit);

	LastHitTime = GetWorld()->GetTimeSeconds();
}

// Called every frame
void APunchBagBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APunchBagBuilding::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Prevent multi hits with an x sec delay
	float CurrentHitTime = GetWorld()->GetTimeSeconds();
	if (HitTimeDelay > (CurrentHitTime - LastHitTime))
	{
		LastHitTime = CurrentHitTime;
		return;
	}

	// Get hit strength
	auto ImpactStrength = NormalImpulse.Size();

	// Play sound and vfx

	// Count Hits if impact was strong enough
	if (ImpactStrength > RequiredHitStrength)
	{
		HitCount++;
		UE_LOG(LogTemp, Warning, TEXT("I'm Hit!"));
	}

	// If HitPoint are exceeded destroy the building!
	if (HitCount >= HitPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ma Ma Mia!"));
		// Hide MovableMesh & Collision
		PhysicsConstraint->SetHiddenInGame(true);
		MovableMesh->SetHiddenInGame(true);
		MovableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MovableMesh->SetSimulatePhysics(false);
		
		// Add destructible component
		// Set Destructible mesh
	}

	LastHitTime = CurrentHitTime;
}

