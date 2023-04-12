// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/Attack.h"
#include "Attack_Ixion_BAExit.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UAttack_Ixion_BAExit : public UAttack
{
	GENERATED_BODY()

public:

	virtual void Start() override;
	virtual void Finish() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Basic Attack: Exit Attack")
	int bulletsExitAttack = 30;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Exit Attack")
	float speedExitAttack = 300.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Exit Attack")
	float lifeSpanExitAttack = 5.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Exit Attack")
	bool useDecelerationCurveExitAttack = true;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Exit Attack")
	UCurveFloat* decelerationCurveExitAttack;

private:

	FTimerHandle fireRateTimerHandle;

	void BAExit();
};
