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
	MovableMesh->SetCollisionProfileName(TEXT("Enemy"));
	MovableMesh->SetEnableGravity(false);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constaint"));
	PhysicsConstraint->SetupAttachment(RootComponent);

	DestructibleComponent2 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Component2"));
	DestructibleComponent2->SetupAttachment(RootComponent);
	//DestructibleMesh->SetSimulatePhysics(false);
	//DestructibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//DestructibleMesh->SetVisibility(false);

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
	UE_LOG(LogTemp, Warning, TEXT("ImpactStrength: %f"), ImpactStrength);
	// Play sound and vfx

	// Count Hits if impact was strong enough
	if (ImpactStrength > RequiredHitStrength)
	{
		HitCount++;
		UE_LOG(LogTemp, Warning, TEXT("I'm Hit!"));
	}

	// If HitPoint are exceeded, destroy the building!
	if (HitCount >= HitPoints)
	{
		FVector Buildinglocation = MovableMesh->GetRelativeLocation();
		FRotator BuildingRotation = MovableMesh->GetRelativeRotation();

		UE_LOG(LogTemp, Warning, TEXT("Building Destroyed!"));
		MovableMesh->DestroyComponent();
		
		DestructibleComponent2->SetSkeletalMesh(DestructibleMesh);
		
		//auto SpawnedDestructibleMesh = NewObject<UDestructibleMesh>(this, MainDestructibleMesh);
		//SpawnedDestructibleMesh->SetSkeletalMesh
		//auto SpawnedDestructibleMesh = GetWorld()->SpawnActor<UDestructibleMesh>(MainDestructibleMesh, Buildinglocation, BuildingRotation);
		/*
		bool BuildingDestroyed = DestroyBuildingMesh();
		if (BuildingDestroyed) {
			SpawnDestructibleMesh();
		}
		*/
	}

	LastHitTime = CurrentHitTime;
}

bool APunchBagBuilding::DestroyBuildingMesh()
{
	// Hide MovableMesh & Collision
	PhysicsConstraint->SetHiddenInGame(true);
	MovableMesh->SetVisibility(false);
	//MovableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MovableMesh->SetSimulatePhysics(false);
	//MovableMesh->SetGenerateOverlapEvents(false);
	MovableMesh->DestroyComponent();

	return true;
}

void APunchBagBuilding::SpawnDestructibleMesh()
{

	//GetWorld()->SpawnActor<UDestructibleComponent>(DestructibleMesh);
	//DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Mesh"));
	//DestructibleMesh->SetupAttachment(RootComponent);
	//DestructibleMesh->SetVisibility(true);
	//DestructibleMesh->SetHiddenInGame(false);
	//DestructibleMesh->SetSimulatePhysics(true);
	//DestructibleMesh->SetCollisionProfileName(TEXT("Destructible"));
}


