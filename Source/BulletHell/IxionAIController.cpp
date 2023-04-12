// Fill out your copyright notice in the Description page of Project Settings.


#include "IxionAIController.h"
#include "Projectile/Projectile.h"
#include "ShooterCharacter.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "BT/Ixion/BTTask_Ixion_BasicAttack.h"

AIxionAIController::AIxionAIController() 
{
}

void AIxionAIController::BeginPlay()
{
    Super::BeginPlay();

    if (baMachineGun) {
        baMachineGun->SetOwner(this);
    }

    if (baBurst) {
        baBurst->SetOwner(this);
    }

    if (baExit) {
        baExit->SetOwner(this);
    }

    if (haVortex) {
        haVortex->SetOwner(this);
    }
}

void AIxionAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (isAttacking && mustLookAtPlayer) {
        LookAtPlayer();
    }
}

void AIxionAIController::StartBasicAttack(const EIxionBasicAttack attack)
{
    isAttacking = true;

    switch(attack) {
        case EIxionBasicAttack::EXIT_ATTACK:
            if (baExit) {
                baExit->Start();
            }
            else{
                FinishAttack();
            }
        break;
        case EIxionBasicAttack::MACHINE_GUN:
            mustLookAtPlayer = true;
            if (baMachineGun) {
                baMachineGun->Start();
            }
            else{
                FinishAttack();
            }
        break;
        case EIxionBasicAttack::BURST:
            mustLookAtPlayer = true;
            if (baBurst) {
                baBurst->Start();
            }
            else{
                FinishAttack();
            }
        break;
        default:
            AttackDefault();
        break;
    }
}

void AIxionAIController::StartHeavyAttack(const EIxionHeavyAttack attack)
{
    isAttacking = true;

    switch(attack) {
        case EIxionHeavyAttack::VORTEX:
            LookAtPlayer();

            if (haVortex) {
                haVortex->Start();
            }
            else{
                FinishAttack();
            }
        break;
        default:
            AttackDefault();
        break;
    }
}

void AIxionAIController::AttackDefault()
{
    UE_LOG(LogTemp, Display, TEXT("AIxionAIController::StartAttack: Invalid attack"));
    isAttacking = false;
    onBasicAttackFinishedDelegate.ExecuteIfBound();
}

void AIxionAIController::FinishAttack(bool isBasicAttack) 
{
    isAttacking = false;
    mustLookAtPlayer = false;

    if (isBasicAttack) {
        onBasicAttackFinishedDelegate.ExecuteIfBound();
    } else {
        onHeavyAttackFinishedDelegate.ExecuteIfBound();
    }
}
