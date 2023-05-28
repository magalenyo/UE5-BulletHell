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
    initialSpeed = characterMovementComponent->MaxFlySpeed;
    characterMovementComponent->MaxFlySpeed = speed;

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

        characterMovementComponent->SetMovementMode(MOVE_Flying);
        characterMovementComponent->Velocity = directionToPosition * characterMovementComponent->GetMaxSpeed();

        FRotator actorRotation = pawn->GetActorRotation();
        FRotator velocityRotation = characterMovementComponent->Velocity.Rotation();
        pawn->SetActorRotation(FRotator(0, velocityRotation.Yaw, 0));
    
        float distanceToPosition = FVector::Distance(pawnLocation, destination);
        if (distanceToPosition <= acceptanceRadius)
        {
            characterMovementComponent->BrakingDecelerationFlying = initialBrakingDeceleration;
            characterMovementComponent->MaxFlySpeed = initialSpeed;
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

