// Fill out your copyright notice in the Description page of Project Settings.


#include "IxionAIController.h"
#include "Projectile/Projectile.h"
#include "ShooterCharacter.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "BT/Ixion/BTTask_Ixion_BasicAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

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

    if (baShockwave) {
        baShockwave->SetOwner(this);
    }

    if (haVortex) {
        haVortex->SetOwner(this);
    }

    if (haShockwaveBullets) {
        haShockwaveBullets->SetOwner(this);
    }

    if (haDescendRush) {
        haDescendRush->SetOwner(this);
    }

    if (baSweep) {
        baSweep->SetOwner(this);
    }

    if (haHomingAndLaser) {
        haHomingAndLaser->SetOwner(this);
    }
}

void AIxionAIController::InitializeBlackboardValues()
{
    // GetBlackboardComponent()->SetValueAsInt(TEXT("Phase"), 1);
    UBlackboardComponent* blackboard = GetBlackboardComponent();
    if (!blackboard) {
        return;
    }

    SetPhase(1);
    blackboard->SetValueAsBool(TEXT("IsFirstTime"), true);
    blackboard->SetValueAsBool(TEXT("IsFirstTimePhase2"), true);
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
        case EIxionBasicAttack::SHOCKWAVE:
            mustLookAtPlayer = true;
            if (baShockwave) {
                baShockwave->Start();
            }
            else{
                FinishAttack();
            }
        break;
        case EIxionBasicAttack::SWEEP:
            //mustLookAtPlayer = true;
            LookAtPlayer();
            if (baSweep) {
                baSweep->Start();
            }
            else {
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
        case EIxionHeavyAttack::SHOCKWAVE_BULLETS:
            if (haShockwaveBullets) {
                haShockwaveBullets->Start();
            }
            else{
                FinishAttack();
            }
        break;
        case EIxionHeavyAttack::DESCEND_RUSH:
            if (haDescendRush) {
                haDescendRush->Start();
            }
            else{
                FinishAttack();
            }
        break;
        case EIxionHeavyAttack::HOMING_AND_LASER:
            LookAtPlayerSameHeight();
            if (haHomingAndLaser) {
                haHomingAndLaser->Start();
            }
            else {
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

void AIxionAIController::SetPhase(int newPhase)
{
    GetBlackboardComponent()->SetValueAsInt(TEXT("Phase"), newPhase);
}

