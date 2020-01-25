// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFlyingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAZYKAIJU_API UEnemyFlyingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFlyingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveAtPlayer();
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float MaxSpeed = 75.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	bool Hover = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float HoverDistance = 150.f;
	


	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UStaticMeshComponent* TargetToSet);

private:
	UStaticMeshComponent* Target = nullptr;
	
	float MovementSpeed = 10.f;
		
};
