// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/Attack.h"
#include "Attack_Ixion_BAMachineGun.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class BULLETHELL_API UAttack_Ixion_BAMachineGun : public UAttack
{
	GENERATED_BODY()

public:

	virtual void Start() override;
	virtual void Finish() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	int bulletsBasicAttack = 70;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float heightOffset = 10.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float radiusOffset = 100.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float nextAttackMinTimeMachineGun = .03f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float nextAttackMaxTimeMachineGun = .06f;

private:

	FTimerHandle fireRateTimerHandle;
	int currentBulletsMachineGun = 0;
	void BAMachineGun();

};
