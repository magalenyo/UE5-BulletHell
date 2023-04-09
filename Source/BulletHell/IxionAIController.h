// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterAIController.h"
#include "Delegates/Delegate.h"
#include "Attacks/Attack_Ixion_BAMachineGun.h"
#include "IxionAIController.generated.h"

UENUM(BlueprintType)
enum class EIxionBasicAttack : uint8
{
	EXIT_ATTACK UMETA(DisplayName = "Exit Attack"),
	MACHINE_GUN UMETA(DisplayName = "Machine Gun Attack"),
	BURST UMETA(DisplayName = "Burst"),
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
	AIxionAIController();

	virtual void Tick(float DeltaSeconds) override;

	// BASIC ATTACKS
	void StartBasicAttack(const EIxionBasicAttack attack);
	void StartHeavyAttack(const EIxionHeavyAttack attack);
	void AttackDefault();
	void BAExit();
	void BAMachineGun();
	void BABurst();
	void HAVortex();

	FOnBasicAttackFinishedSignature onBasicAttackFinishedDelegate;
	FOnHeavyAttackFinishedSignature onHeavyAttackFinishedDelegate;
	
protected:

	virtual void BeginPlay() override;

private:

	// BA: Machine Gun
	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	int bulletsBasicAttack = 30;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float heightOffset = 10.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float radiusOffset = 100.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float nextAttackMinTimeMachineGun = .03f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Machine Gun")
	float nextAttackMaxTimeMachineGun = .06f;

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

	// BA: Burst Attack
	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	int bulletsPerWaveBurstAttack = 4;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	int wavesBurstAttack = 6;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	float durationBurstAttack = .3f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	float angleBurstAttack = 40.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	float speedBurstAttack = 800.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Burst")
	float delayToHomeBurstAttack = .5f;

	// HA: VORTEX
	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	int startPointsPerWaveVortex = 12;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	int wavesPerPointVortex = 24;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	float speedVortexAttack = 800.0f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	float durationVortex = .85f;

	UPROPERTY(EditAnywhere, Category="Heavy Attack: Vortex")
	float rotationSpeedVortex = 2.0f;

	// UPROPERTY(EditAnywhere, Category="test")
	// UAttack_Ixion_BAMachineGun* attacktest;

	// ATTRIBUTES 
	bool isAttacking = false;

	bool mustLookAtPlayer = false;

	FTimerHandle fireRateTimerHandle;

	int currentBulletsMachineGun = 0;

	int currentWaveBurst = 0;

	int currentWaveVortex = 0;

	int currentWaveVortexReposition = 0;

	FTimerHandle retargetWaveVortexTimerHandle;

	std::vector<class AProjectile*> projectilesVortex;

	FVector targetPositionVortex = FVector::Zero();

	std::vector<class AProjectile*> projectilesBurst;

	void FinishAttack(bool isBasicAttack = true);

	void RepositionVortexProjectiles();

};
