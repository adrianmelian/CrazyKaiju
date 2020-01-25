// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAimComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAZYKAIJU_API UEnemyAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyAimComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AimAtPlayer();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float RotateSpeed = 1.f;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UStaticMeshComponent* TargetToSet);
		
private:
	UStaticMeshComponent* Target = nullptr;
};
