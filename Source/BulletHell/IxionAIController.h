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

UENUM(BlueprintType)
enum class EIxionHeavyAttack : uint8
{
	VORTEX UMETA(DisplayName = "Vortex"),
	COUNT
};

DECLARE_DELEGATE(FOnBasicAttackFinishedSignature);
DECLARE_DELEGATE(FOnHeavyAttackFinishedSignature);

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
	void StartHeavyAttack(const EIxionHeavyAttack attack);
	void AttackDefault();
	void BAExit();
	void BAMachineGun();
	void HAVortex();

	FOnBasicAttackFinishedSignature onBasicAttackFinishedDelegate;
	FOnHeavyAttackFinishedSignature onHeavyAttackFinishedDelegate;
	
protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Combat")
	bool usePredicted = false;

	// BA: Machine Gun
	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	int bulletsBasicAttack = 30;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float heightOffset = 10.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float radiusOffset = 100.0f;

	// BA: Exit Attack
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

	// HA: VORTEX
	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	int startPointsPerWaveVortex = 12;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	int wavesPerPointVortex = 24;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Exit Attack")
	float speedVortexAttack = 800.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Exit Attack")
	float durationVortex = 4.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Exit Attack")
	float rotationSpeedVortex = 4.0f;

	// ATTRIBUTES 
	bool isAttacking = false;

	bool mustLookAtPlayer = false;

	FTimerHandle fireRateTimerHandle;

	int currentBulletsBasicAttack = 0;

	int currentWaveVortex = 0;

	const FVector GetPredictedDestination() const;

	void FinishAttack(bool isBasicAttack = true);

};
