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
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot();

	UFUNCTION(BlueprintPure)		// It does blueprint callable + other stuff. A pure nonde is a node that doesn't have an execution pin, it only has a result
	bool IsDead() const;

	UFUNCTION(BlueprintPure)		// It does blueprint callable + other stuff. A pure nonde is a node that doesn't have an execution pin, it only has a result
	float GetHealthPercent() const;

	// COMPONENTS GETTERS
	const USceneComponent* GetProjectileSpawnPoint() const;

	const float GetMovementSpeed() const;

private:
	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* projectileSpawnPoint;

	// PROPERTIES
	UPROPERTY(EditAnywhere)
	float rotationRate = 40;

	UPROPERTY(EditAnywhere)
	float mouseRotationSpeedHorizontal = 20;

	UPROPERTY(EditAnywhere)
	float mouseRotationSpeedVertical = 15;

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
	

	void MoveForward(float AxisValue);	// No additional methods needed for controller input since it already is suported for both (it takes a rate instead of a value)
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookUpRate(float AxisValue);	// Used for controller input only
	void LookRight(float AxisValue);
	void LookRightRate(float AxisValue);
	void JumpUp();
	void StartSprint();
	void StopSprint();

	bool isSprinting = false;
};
