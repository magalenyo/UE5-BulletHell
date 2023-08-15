// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/Attack.h"
#include "Attack_Ixion_HAVortex.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UAttack_Ixion_HAVortex : public UAttack
{
	GENERATED_BODY()
	
public:

	virtual void Start() override;
	virtual void Finish() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	int startPointsPerWaveVortex = 12;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	int wavesPerPointVortex = 24;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	float speedVortexAttack = 1100.0f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	float durationVortex = .85f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	float rotationSpeedVortex = 2.0f;

private:

	int currentWaveVortex = 0;
	int currentWaveVortexReposition = 0;
	FVector targetPositionVortex = FVector::Zero();
	FTimerHandle fireRateTimerHandle;
	FTimerHandle retargetWaveVortexTimerHandle;
	std::vector<class AProjectile*> projectilesVortex;

	void HAVortex();
	void RepositionVortexProjectiles();
};
