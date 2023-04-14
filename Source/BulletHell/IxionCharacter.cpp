// Fill out your copyright notice in the Description page of Project Settings.


#include "IxionCharacter.h"
#include "IxionAIController.h"

void AIxionCharacter::HandleDeath()
{
    if (!IsDead()) {
        return;
    }

    if (phase == 1) {
        UE_LOG(LogTemp, Display, TEXT("Change Phase 1"));
        ChangePhase(2);
        return;
    }
    
    if (phase == 2) {
        UE_LOG(LogTemp, Display, TEXT("Change Phase 2"));
        ChangePhase(-1);
        DisableCharacter();
	}
}

void AIxionCharacter::ChangePhase(int newPhase)
{
    phase = newPhase;

    AIxionAIController* controller = Cast<AIxionAIController>(GetController());

    controller->SetPhase(newPhase);

    if (phase == 2) {
        health = maxHealth;
        UE_LOG(LogTemp, Display, TEXT("Change Phase 2: %f"), health);
    }

    // HANDLE UI Change
}
