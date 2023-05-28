// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/Attack.h"
#include "Attack_Ixion_BAShockwave.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UAttack_Ixion_BAShockwave : public UAttack
{
	GENERATED_BODY()
	
public:

	virtual void Start() override;
	virtual void Finish() override;

private:

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	float speed = 8.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	float duration = 10.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	float hitCooldown = .3f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	TSubclassOf<class AAttackBehavior_Ixion_BAShockwave> shockwaveClass;
	
};
