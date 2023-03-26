// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTask_FlyTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTask_FlyTo::UBTTask_FlyTo()
{
    NodeName = TEXT("Fly To");
    bNotifyTick = true;
    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_FlyTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    characterMovementComponent = GetCharacterMovement(OwnerComp);
    if (!characterMovementComponent) {
        return EBTNodeResult::Failed;
    }

    initialBrakingDeceleration = characterMovementComponent->BrakingDecelerationFlying;
    characterMovementComponent->BrakingDecelerationFlying = maxBrakingDeceleration;

    return EBTNodeResult::InProgress;
}

void UBTTask_FlyTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* controller = OwnerComp.GetAIOwner();
    APawn* pawn = controller->GetPawn();

    FVector destination = GetDestination(OwnerComp);

    if (!destination.IsZero())
    {
        FVector pawnLocation = pawn->GetActorLocation();
        FVector directionToPosition = (destination - pawnLocation).GetSafeNormal();

        FRotator currentRotation = pawn->GetActorRotation();
        FRotator newRotation = FRotator::MakeFromEuler(FVector(currentRotation.Pitch, currentRotation.Roll, directionToPosition.Rotation().Yaw));
        pawn->SetActorRotation(newRotation);

        characterMovementComponent->SetMovementMode(MOVE_Flying);
        characterMovementComponent->Velocity = directionToPosition * characterMovementComponent->GetMaxSpeed();
    
        float distanceToPosition = FVector::Distance(pawnLocation, destination);
        if (distanceToPosition <= acceptanceRadius)
        {
            characterMovementComponent->BrakingDecelerationFlying = initialBrakingDeceleration;
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    }
}

FVector UBTTask_FlyTo::GetDestination(const UBehaviorTreeComponent& OwnerComp) const
{
    return OwnerComp.GetBlackboardComponent()->GetValueAsVector(destinationKey.SelectedKeyName);
}

UCharacterMovementComponent* UBTTask_FlyTo::GetCharacterMovement(const UBehaviorTreeComponent& OwnerComp) const
{
    AAIController* controller = OwnerComp.GetAIOwner();

    if (!controller) {
        return nullptr;
    }

    APawn* pawn = controller->GetPawn();

    if (!pawn) {
        return nullptr;
    }

    return pawn->FindComponentByClass<UCharacterMovementComponent>();
}
