// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	bool IsDead() const;

	const float GetFlyOffset();
	
protected:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> projectileClass;

	APawn* playerPawn;

	virtual void BeginPlay() override;

	const USceneComponent* GetProjectileSpawnPoint() const;

	const FRotator LookAt(const FVector target) const;

	void LookAtPlayer();

private:
	UPROPERTY(EditAnywhere)
	float flyOffset = 500;

};
