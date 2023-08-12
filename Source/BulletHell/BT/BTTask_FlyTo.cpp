// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTask_FlyTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IxionAIController.h"
#include "IxionCharacter.h"


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

    StartTrail(OwnerComp);

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
            StopTrail(OwnerComp);
            characterMovementComponent->BrakingDecelerationFlying = initialBrakingDeceleration;
            characterMovementComponent->MaxFlySpeed = initialSpeed;
            characterMovementComponent->Velocity = FVector(0, 0, 0);
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

void UBTTask_FlyTo::StartTrail(const UBehaviorTreeComponent& OwnerComp) const
{
    AIxionAIController* character = Cast<AIxionAIController>(OwnerComp.GetAIOwner());
    if (!character) {
        return;
    }

    AIxionCharacter* pawn = Cast<AIxionCharacter>(character->GetPawn());
    if (!pawn) {
        return;
    }

    if (pawn) {
        pawn->StartTrail();
    }
}

void UBTTask_FlyTo::StopTrail(const UBehaviorTreeComponent& OwnerComp) const
{
    AIxionAIController* character = Cast<AIxionAIController>(OwnerComp.GetAIOwner());
    if (!character) {
        return;
    }

    AIxionCharacter* pawn = Cast<AIxionCharacter>(character->GetPawn());
    if (!pawn) {
        return;
    }

    if (pawn) {
        pawn->StopTrail();
    }
}

