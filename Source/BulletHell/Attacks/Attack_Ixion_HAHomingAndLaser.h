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
	UPROPERTY(EditAnywhere)
	float bulletsStartTime = .8f;

	UPROPERTY(EditAnywhere)
	float bulletsDuration = 5.0f;

	UPROPERTY(EditAnywhere)
	int waves = 6;
	
	UPROPERTY(EditAnywhere)
	float bulletsConeAngle = 110.0f;

	UPROPERTY(EditAnywhere)
	float bulletsConeAngleVertical = 35;

	UPROPERTY(EditAnywhere)
	int bulletsVertical = 2;

	UPROPERTY(EditAnywhere)
	int bulletsPerSide = 24;

	UPROPERTY(EditAnywhere)
	int bulletsPerWave = 12;

	UPROPERTY(EditAnywhere)
	float bulletsSpeed = 1400.0f;

	UPROPERTY(EditAnywhere)
	float bulletsHomingMagnitude = 2200.0f;

	UPROPERTY(EditAnywhere)
	float bulletsMinTimeWave = .03f;

	UPROPERTY(EditAnywhere)
	float bulletsMaxTimeWave = .06f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> homingProjectileClass;

	UPROPERTY(EditAnywhere)
	FRotator test;


	int currentWave = 0;
	FTimerHandle fireRateTimerHandle;

	void FireBullets();
};
