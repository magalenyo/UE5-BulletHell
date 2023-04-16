// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterAIController.h"
#include "Delegates/Delegate.h"
#include "Attacks/Attack_Ixion_BAMachineGun.h"
#include "Attacks/Attack_Ixion_BABurst.h"
#include "Attacks/Attack_Ixion_BAExit.h"
#include "Attacks/Attack_Ixion_BAShockwave.h"
#include "Attacks/Attack_Ixion_HAVortex.h"
#include "IxionAIController.generated.h"

UENUM(BlueprintType)
enum class EIxionBasicAttack : uint8
{
	EXIT_ATTACK,
	MACHINE_GUN,
	BURST,
	SHOCKWAVE,
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

	// ATTACKS
	void StartBasicAttack(const EIxionBasicAttack attack);
	void StartHeavyAttack(const EIxionHeavyAttack attack);
	void AttackDefault();
	void BAExit();
	void BAMachineGun();
	void BABurst();
	void HAVortex();
	void FinishAttack(bool isBasicAttack = true);
	void SetPhase(int newPhase);

	FOnBasicAttackFinishedSignature onBasicAttackFinishedDelegate;
	FOnHeavyAttackFinishedSignature onHeavyAttackFinishedDelegate;
	
protected:

	virtual void BeginPlay() override;
	virtual void InitializeBlackboardValues() override;

private:

	UPROPERTY(Instanced, EditAnywhere, Category = Test)
	UAttack_Ixion_BAMachineGun* baMachineGun;

	UPROPERTY(Instanced, EditAnywhere, Category = Test)
	UAttack_Ixion_BABurst* baBurst;

	UPROPERTY(Instanced, EditAnywhere, Category = Test)
	UAttack_Ixion_BAExit* baExit;

	UPROPERTY(Instanced, EditAnywhere, Category = Test)
	UAttack_Ixion_BAShockwave* baShockwave;

	UPROPERTY(Instanced, EditAnywhere, Category = Test)
	UAttack_Ixion_HAVortex* haVortex;

	// ATTRIBUTES 
	bool isAttacking = false;
	bool mustLookAtPlayer = false;

};
