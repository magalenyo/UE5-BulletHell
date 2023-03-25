// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetMovementState.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SetMovementState::UBTTask_SetMovementState()
{
    NodeName = TEXT("Set Movement State");
}

EBTNodeResult::Type UBTTask_SetMovementState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* aiOwner = OwnerComp.GetAIOwner(); // to get the controller

    if (!aiOwner) {
        return EBTNodeResult::Failed;
    }

    APawn* pawn = aiOwner->GetPawn(); // to get the actor

    if (pawn)
    {
        UCharacterMovementComponent* movementComponent = pawn->FindComponentByClass<UCharacterMovementComponent>();
        if (movementComponent)
        {
            movementComponent->SetMovementMode(movementMode);
        }
    }
    
    return EBTNodeResult::Succeeded;
}

