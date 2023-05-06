// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class BULLETHELL_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot();

	UFUNCTION(BlueprintPure)		// It does blueprint callable + other stuff. A pure nonde is a node that doesn't have an execution pin, it only has a result
	bool IsDead() const;

	UFUNCTION(BlueprintPure)		// It does blueprint callable + other stuff. A pure nonde is a node that doesn't have an execution pin, it only has a result
	float GetHealthPercent() const;

	// COMPONENTS GETTERS
	const USceneComponent* GetProjectileSpawnPoint() const;

	const float GetMovementSpeed() const;

protected:

	// COMPONENTS
	UPROPERTY(EditAnywhere);
	USceneComponent* projectileSpawnPoint;

	// PROPERTIES
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> gunClass;

	UPROPERTY()
	AGun* gun;

	UPROPERTY(EditAnywhere)
	float movementSpeed = 100;

	UPROPERTY(EditDefaultsOnly)
	float maxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float health;

	UPROPERTY(EditAnywhere, Category="Sprint")
	float sprintMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, Category="Sprint")
	float jumpMultiplier = 1.5f;

	bool isSprinting = false;

	virtual void BeginPlay() override;

	void StartSprint();
	void StopSprint();
	virtual void HandleDeath();
	virtual void DisableCharacter();
};
