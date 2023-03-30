// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterAIController.h"
#include "Delegates/Delegate.h"
#include "IxionAIController.generated.h"


DECLARE_DELEGATE(FOnBasicAttackFinishedSignature);

/**
 * 
 */
UCLASS()
class BULLETHELL_API AIxionAIController : public AShooterAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	void StartBasicAttack();
	void BasicAttack();

	FOnBasicAttackFinishedSignature onBasicAttackFinishedDelegate;
	
protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Combat")
	int bulletsBasicAttack = 30;

	UPROPERTY(EditAnywhere, Category="Combat")
	float heightOffset = 10.0f;

	UPROPERTY(EditAnywhere, Category="Combat")
	float radiusOffset = 100.0f;

	UPROPERTY(EditAnywhere)
	bool usePredicted = false;
	
	bool isAttacking = false;

	FTimerHandle fireRateTimerHandle;

	int currentBulletsBasicAttack = 0;

	const FVector GetPredictedDestination() const;

	
};
