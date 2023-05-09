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

private:
	UPROPERTY(EditAnywhere)
	float duration = 8.0f;

	UPROPERTY(EditAnywhere)
	bool drawDebug = false;

	UPROPERTY(EditAnywhere)
	UCurveFloat* accelerationCurve;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> splineClass;

	AActor* spline;

};
