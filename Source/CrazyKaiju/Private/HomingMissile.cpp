// Fill out your copyright notice in the Description page of Project Settings.

#include "HomingMissile.h"

#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AHomingMissile::AHomingMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct Static Mesh Component
	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileMesh"));
	SetRootComponent(MissileMesh);

	// Construct Projectile Movement Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = MissileMesh;
	ProjectileMovement->InitialSpeed = 200.f;
	ProjectileMovement->MaxSpeed = 300.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->HomingAccelerationMagnitude = 0.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->Velocity = FVector(0, 0, 0);

	// Particles
	LaunchBlastParticles = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast Particles"));
	LaunchBlastParticles->AttachTo(MissileMesh);

	ImpactBlastParticles = CreateDefaultSubobject<UParticleSystemComponent>(FName("Immpact Blast Particles"));
	ImpactBlastParticles->AttachToComponent(MissileMesh, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlastParticles->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AHomingMissile::BeginPlay()
{
	Super::BeginPlay();
	
	MissileMesh->OnComponentHit.AddDynamic(this, &AHomingMissile::OnHit);
	//MissileMesh->OnComponentBeginOverlap.AddDynamic(this, &AHomingMissile::OnOverlapBegin);
}

// Called every frame
void AHomingMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto ThisWorld = GetWorld();
	auto PlayerPawn = ThisWorld->GetFirstPlayerController()->GetPawn();
	if (!ensure(MissileMesh && PlayerPawn)) { return; }

	float DeltaSecs = ThisWorld->DeltaTimeSeconds;
	auto PlayerHeadLoc = PlayerPawn->GetActorLocation();
	PlayerHeadLoc.Z += 100.f;
	auto WantedDir = (PlayerHeadLoc - GetActorLocation()).GetSafeNormal();
	ProjectileMovement->Velocity += (WantedDir * MissileSpeed) * DeltaSecs;
	ProjectileMovement->Velocity = ProjectileMovement->Velocity;
}

void AHomingMissile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Something!"));
	Destroy();
}

void AHomingMissile::OnOverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped Something!"));
	Destroy();
}