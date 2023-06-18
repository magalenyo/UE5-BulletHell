// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/Attack.h"
#include "Attack_Ixion_HAHomingAndLaser.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UAttack_Ixion_HAHomingAndLaser : public UAttack
{
	GENERATED_BODY()

public:

	virtual void Start() override;
	virtual void Finish() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	float bulletsStartTime = .8f;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	float bulletsDuration = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	int waves = 6;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	float bulletsConeAngle = 110.0f;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	float bulletsConeAngleVertical = 35;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	int bulletsVertical = 2;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	int bulletsPerSide = 24;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	int bulletsPerWave = 12;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	float bulletsSpeed = 1400.0f;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	float bulletsHomingMagnitude = 2200.0f;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	float bulletsMinTimeWave = .03f;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	float bulletsMaxTimeWave = .06f;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: HomingAndLaser - Bullets")
	float laserDuration = 1.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> homingProjectileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAttackBehavior_Ixion_HALaserBeam> laserClass;


	int currentWave = 0;
	FTimerHandle fireRateTimerHandle;
	FTimerHandle laserTimerHandle;

	void FireLaserBeam();
	void FireBullets();
};
