// Fill out your copyright notice in the Description page of Project Settings.

#include "HomingMissile.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VRCharacter.h"

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
	LaunchBlastParticles->AttachToComponent(MissileMesh, FAttachmentTransformRules::KeepWorldTransform);

	ImpactBlastParticles = CreateDefaultSubobject<UParticleSystemComponent>(FName("Immpact Blast Particles"));
	ImpactBlastParticles->AttachToComponent(MissileMesh, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlastParticles->bAutoActivate = false;

	// Explosion Force
	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AHomingMissile::BeginPlay()
{
	Super::BeginPlay();
	
	MissileMesh->OnComponentHit.AddDynamic(this, &AHomingMissile::OnHit);
}

// Called every frame
void AHomingMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlyAtPlayer();
}

void AHomingMissile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	ImpactBlastParticles->Activate(); // Turn on hit explosion
	LaunchBlastParticles->Deactivate(); // Turn off trailing smoke
	ExplosionForce->FireImpulse(); // Apply force to hit actor

	UGameplayStatics::ApplyRadialDamage(this, MissileDamage, GetActorLocation(), ExplosionForce->Radius, TSubclassOf<UDamageType>(), TArray<AActor*>());
	UE_LOG(LogTemp, Warning, TEXT("Hit Something!"));

	Destroy();
}

void AHomingMissile::FlyAtPlayer()
{
	auto ThisWorld = GetWorld();
	auto PlayerPawn = ThisWorld->GetFirstPlayerController()->GetPawn();
	if (!ensure(MissileMesh && PlayerPawn)) { return; }

	float DeltaSecs = ThisWorld->DeltaTimeSeconds;
	auto PlayerHeadLoc = PlayerPawn->GetActorLocation();
	PlayerHeadLoc.Z += 100.f;
	auto WantedDir = (PlayerHeadLoc - GetActorLocation()).GetSafeNormal();
	WantedDir += PlayerPawn->GetVelocity() * WantedDir.Size() / MissileSpeed;
	ProjectileMovement->Velocity += (WantedDir * MissileSpeed) * DeltaSecs;
	ProjectileMovement->Velocity = ProjectileMovement->Velocity.GetSafeNormal() * MissileSpeed;

	FRotator DeltaRot = UKismetMathLibrary::FindLookAtRotation(MissileMesh->GetComponentLocation(), PlayerHeadLoc);
	FRotator DesiredRot = UKismetMathLibrary::RInterpTo(MissileMesh->GetComponentRotation(), DeltaRot, DeltaSecs, 0.5f);
	MissileMesh->SetWorldRotation(DesiredRot);
}
