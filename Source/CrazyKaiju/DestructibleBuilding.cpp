// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleBuilding.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"
#include "Engine.h"


// Sets default values
ADestructibleBuilding::ADestructibleBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destrcutible Component"));
	DestructibleComponent->SetupAttachment(RootComponent);
	DestructibleComponent->SetNotifyRigidBodyCollision(true);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Component"));
	TriggerComponent->SetupAttachment(RootComponent);

	IsDestroyed = false;
	IsTriggerEnabled = false;

	MaxHealth = 10.f;

	DefaultDamage = 1.f;
	DefaultImpulse = 1.f;
}

// Called when the game starts or when spawned
void ADestructibleBuilding::BeginPlay()
{
	Super::BeginPlay();

	DestructibleComponent->OnComponentHit.AddDynamic(this, &ADestructibleBuilding::Damage);

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestructibleBuilding::Trigger);

	CurrentHealth = MaxHealth;
}

void ADestructibleBuilding::Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp->ComponentHasTag("Weapon"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Hit By: " + OtherComp->GetName());

		CurrentHealth = CurrentHealth - 1.f;
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Current Health: " + FString::SanitizeFloat(CurrentHealth));

		if (CurrentHealth <= 0.f)
		{
			Destroy(DefaultDamage, Hit.Location, NormalImpulse, DefaultImpulse);
		}
	}
}

void ADestructibleBuilding::Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsTriggerEnabled && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);

		Destroy(DefaultDamage, DestructibleComponent->GetComponentLocation(), OtherActor->GetActorForwardVector(), DefaultImpulse);
	}
}


void ADestructibleBuilding::Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, __FUNCTION__);
	//DestructibleComponent->ApplyDamage(Damage*100, HitLocation, ImpulseDir, Impulse*100);
	DestructibleComponent->AddForceAtLocation(ImpulseDir, HitLocation);

	if (!IsDestroyed)
	{
		IsDestroyed = true;

	}
}



// Called every frame
void ADestructibleBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

