// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/Attack.h"
#include "Attack_Ixion_HADescendRush.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UAttack_Ixion_HADescendRush : public UAttack
{
	GENERATED_BODY()

public:
	virtual void Start() override;

	UFUNCTION()
	virtual void Finish() override;

	UFUNCTION()
	void OnPerformAttack();

private:
	UPROPERTY(EditAnywhere)
	float duration = 8.0f;

	UPROPERTY(EditAnywhere)
	bool drawDebug = false;

	UPROPERTY(EditAnywhere)
	float attackPoint = .15f;

	UPROPERTY(EditAnywhere)
	UCurveFloat* accelerationCurve;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> splineClass;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	int startPointsPerWaveVortex = 12;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	int wavesPerPointVortex = 24;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	float speedVortexAttack = 1500.0f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	float durationVortex = .85f;

	UPROPERTY(EditAnywhere, Category = "Heavy Attack: Vortex")
	float durationExpanseVortex = .45f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	float rotationSpeedVortex = 2.0f;

	AActor* spline;

	class UComponentFollowSpline* followSplineComponent;

	int currentWaveVortex = 0;
	int currentWaveVortexReposition = 0;
	FVector fireLocation = FVector::Zero();
	FVector targetLocation = FVector::Zero();
	FVector targetPositionVortex = FVector::Zero();
	FTimerHandle fireRateTimerHandle;
	FTimerHandle retargetWaveVortexTimerHandle;
	std::vector<class AProjectile*> projectilesVortex;

	void FireVortex();

	void RepositionVortexProjectiles();
};
