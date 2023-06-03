// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/Attack.h"
#include "Attack_Ixion_BASweep.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UAttack_Ixion_BASweep : public UAttack
{
	GENERATED_BODY()
	
public:

	virtual void Start() override;
	virtual void Finish() override;

private:

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	float durationPerWave = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	int bulletsPerWave = 32;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	int waves = 4;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	int lines = 3;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	float bulletSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	float horizontalAngle = 75.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	float verticalAngle = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	FRotator angleOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	float extraBulletOffsetDegree = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Sweep")
	float extraBulletTime = .12f;

	float nextTime = 0;
	float horizontalAnglePerBullet = 0;
	float verticalAnglePerBullet = 0;
	int direction = 1;
	float currentWave = 0;
	float currentProjectile = 1;
	FTimerHandle fireRateTimerHandle;
	FVector spawnPoint = FVector::ZeroVector;

	void BASweep();
};
