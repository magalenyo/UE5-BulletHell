// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterAIController.h"
#include "Delegates/Delegate.h"
#include "Attacks/Attack_Ixion_BAMachineGun.h"
#include "Attacks/Attack_Ixion_BABurst.h"
#include "Attacks/Attack_Ixion_BAExit.h"
#include "Attacks/Attack_Ixion_HAVortex.h"
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

	// ATTACKS
	void StartBasicAttack(const EIxionBasicAttack attack);
	void StartHeavyAttack(const EIxionHeavyAttack attack);
	void AttackDefault();
	void BAExit();
	void BAMachineGun();
	void BABurst();
	void HAVortex();
	void FinishAttack(bool isBasicAttack = true);

	FOnBasicAttackFinishedSignature onBasicAttackFinishedDelegate;
	FOnHeavyAttackFinishedSignature onHeavyAttackFinishedDelegate;
	
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(Instanced, EditAnywhere, Category = Test)
	UAttack_Ixion_BAMachineGun* baMachineGun;

	UPROPERTY(Instanced, EditAnywhere, Category = Test)
	UAttack_Ixion_BABurst* baBurst;

	UPROPERTY(Instanced, EditAnywhere, Category = Test)
	UAttack_Ixion_BAExit* baExit;

	UPROPERTY(Instanced, EditAnywhere, Category = Test)
	UAttack_Ixion_HAVortex* haVortex;

	// ATTRIBUTES 
	bool isAttacking = false;
	bool mustLookAtPlayer = false;

};
