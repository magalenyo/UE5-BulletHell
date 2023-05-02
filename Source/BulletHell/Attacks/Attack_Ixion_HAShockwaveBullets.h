// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/Attack.h"
#include "Attack_Ixion_HAShockwaveBullets.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UAttack_Ixion_HAShockwaveBullets : public UAttack
{
	GENERATED_BODY()

public:

	virtual void Start() override;
	virtual void Finish() override;

private:

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Shockwave")
	float duration = 12.0f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Shockwave")
	float shockwaveSpeed = 9.0f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Shockwave")
	float shockwaveZSpeed = 16.0f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Shockwave")
	int shockwaveNumber = 4;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Shockwave")
	float shockwaveHitCooldown = .3f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Shockwave")
	float shockwaveDuration = 4.5f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Shockwave")
	TSubclassOf<class AAttackBehavior_Ixion_BAShockwave> shockwaveClass;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Bullets")
	int bulletsStartPoints = 22;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Bullets")
	int bulletsPerPoints = 2;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Bullets")
	float bulletsSpeed = 800.0f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Bullets")
	float bulletsWaveDelay = .2f;

	// UPROPERTY(EditAnywhere, Category="Heavy Attack: ShockwaveBullets - Bullets")
	// TSubclassOf<class AAttackBehavior_Ixion_BAShockwave> shockwaveClass;

	const USceneComponent* projectileSpawnPointTop;
	const USceneComponent* projectileSpawnPointBottom;

	FTimerHandle shockwaveFireRateTimerHandle;
	FTimerHandle bulletsFireRateTimerHandle;

	void BAShockwave();

	void FireShockwave();
	void FireBulletsWave();
};
