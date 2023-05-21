// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_BAShockwave.h"
#include "AttacksBehaviors/AttackBehavior_Ixion_BAShockwave.h"
#include "IxionAIController.h"


void UAttack_Ixion_BAShockwave::Start()
{
    if (shockwaveClass) {
        AAttackBehavior_Ixion_BAShockwave* shockwave = GetWorld()->SpawnActor<AAttackBehavior_Ixion_BAShockwave>(shockwaveClass);
        if (shockwave) {
            shockwave->SetLifeSpan(duration);
            shockwave->SetSpeed(speed);
            shockwave->SetScaleGrow(true);
        }
    }
    Finish();
}

void UAttack_Ixion_BAShockwave::Finish()
{
    AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
    if (ixion) {
        ixion->FinishAttack();
    }
}

