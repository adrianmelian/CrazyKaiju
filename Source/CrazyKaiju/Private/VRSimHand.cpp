// Fill out your copyright notice in the Description page of Project Settings.

#include "VRSimHand.h"

// Sets default values
AVRSimHand::AVRSimHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Collision->SetEnableGravity(false);
	Collision->SetSimulatePhysics(true);
	Collision->SetNotifyRigidBodyCollision(true);

	DebugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Debug Mesh"));
	DebugMesh->SetupAttachment(Collision);
}

// Called when the game starts or when spawned
void AVRSimHand::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentHit.AddDynamic(this, &AVRSimHand::OnHit);
}

// Called every frame
void AVRSimHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRSimHand::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UDestructibleComponent* Building = Cast<UDestructibleComponent>(OtherComp);
	if (Building == nullptr) { return; }

	Building->ApplyRadiusDamage(BaseDamage, Hit.Location, DamageRadius, ImpulseStrength, false);

}

