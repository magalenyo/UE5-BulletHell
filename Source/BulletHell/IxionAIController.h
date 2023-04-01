// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterAIController.h"
#include "Delegates/Delegate.h"
#include "IxionAIController.generated.h"

UENUM(BlueprintType)
enum class EIxionBasicAttack : uint8
{
	EXIT_ATTACK UMETA(DisplayName = "Exit Attack"),
	MACHINE_GUN UMETA(DisplayName = "Machine Gun Attack"),
	COUNT
};

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

	// BASIC ATTACKS
	void StartBasicAttack(const EIxionBasicAttack attack);
	void BADefault();
	void BAExit();
	void BAMachineGun();

	FOnBasicAttackFinishedSignature onBasicAttackFinishedDelegate;
	
protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	int bulletsBasicAttack = 30;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float heightOffset = 10.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float radiusOffset = 100.0f;

	UPROPERTY(EditAnywhere)
	bool usePredicted = false;

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
	
	bool isAttacking = false;

	bool mustLookAtPlayer = false;

	FTimerHandle fireRateTimerHandle;

	int currentBulletsBasicAttack = 0;

	const FVector GetPredictedDestination() const;

	void FinishAttack();

	
};
