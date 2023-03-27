// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterAIController.h"
#include "IxionAIController.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API AIxionAIController : public AShooterAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	void BasicAttack();
	
protected:

	virtual void BeginPlay() override;
	
};
