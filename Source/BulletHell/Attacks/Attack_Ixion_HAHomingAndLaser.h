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
	float bulletsDuration = 10.0f;

	UPROPERTY(EditAnywhere)
	int bulletsStartPoints = 8;

	UPROPERTY(EditAnywhere)
	int bulletsPerStartPoint = 8;
	
	UPROPERTY(EditAnywhere)
	float bulletsConeAngle = 90.0f;

	UPROPERTY(EditAnywhere)
	float bulletsSpeed = 1200.0f;

	UPROPERTY(EditAnywhere)
	float bulletsHomingMagnitude = 1400.0f;

	FTimerHandle fireRateTimerHandle;

	void FireBullets();
};
