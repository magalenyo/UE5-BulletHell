// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/Attack.h"
#include "Attack_Ixion_BABurst.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UAttack_Ixion_BABurst : public UAttack
{
	GENERATED_BODY()
	
public:

	virtual void Start() override;
	virtual void Finish() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	int bulletsPerWaveBurstAttack = 5;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	int wavesBurstAttack = 8;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	float durationBurstAttack = .3f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	float angleBurstAttack = 30.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	float speedBurstAttack = 1200.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	float delayToHomeBurstAttack = .5f;

private:

	FTimerHandle fireRateTimerHandle;
	int currentWaveBurst = 0;
	std::vector<class AProjectile*> projectilesBurst;

	void BABurst();
};
