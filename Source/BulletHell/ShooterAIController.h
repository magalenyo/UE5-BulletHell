// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class AProjectile;

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
	APawn* GetPlayerPawn();
	const TSubclassOf<AProjectile> GetProjectileClass() const;

	const FRotator LookAt(const FVector target) const;
	const FRotator LookAt(const FVector source, const FVector target) const;
	void LookAtPlayer();
	
protected:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> projectileClass;

	APawn* playerPawn;

	virtual void BeginPlay() override;

	const USceneComponent* GetProjectileSpawnPoint() const;

private:
	UPROPERTY(EditAnywhere)
	float flyOffset = 500;

};
