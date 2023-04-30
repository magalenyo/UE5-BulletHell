// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTask_Drop.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Drop::UBTTask_Drop()
{
    NodeName = TEXT("Drop");
    bNotifyTick = true;
    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Drop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* controller = OwnerComp.GetAIOwner();
    APawn* pawn = controller->GetPawn();

    characterMovementComponent = pawn->FindComponentByClass<UCharacterMovementComponent>();
    if (!characterMovementComponent) {
        return EBTNodeResult::Failed;
    }

	FCollisionQueryParams params;
	// params.AddIgnoredActor(this);
    params.AddIgnoredActor(controller);
    params.AddIgnoredActor(pawn);

    FHitResult hit;
    bool hasHit = GetWorld()->LineTraceSingleByChannel(
        hit,
		pawn->GetActorLocation(),
		pawn->GetActorLocation() - FVector(0, 0, dropDistance),
		ECollisionChannel::ECC_WorldStatic,
		params
	);

    if (!hasHit) {
        return EBTNodeResult::Succeeded;        // Should perhaps be aborted?
    }

    targetPosition = hit.ImpactPoint;
    characterMovementComponent->SetMovementMode(MOVE_Flying);

    initialBrakingDeceleration = characterMovementComponent->BrakingDecelerationFlying;
    characterMovementComponent->BrakingDecelerationFlying = maxBrakingDeceleration;
    initialSpeed = characterMovementComponent->MaxFlySpeed;
    characterMovementComponent->MaxFlySpeed = speed;

    return EBTNodeResult::InProgress;
}

void UBTTask_Drop::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* controller = OwnerComp.GetAIOwner();
    APawn* pawn = controller->GetPawn();

    FVector pawnLocation = pawn->GetActorLocation();
    FVector directionToPosition = (targetPosition - pawnLocation).GetSafeNormal();

    characterMovementComponent->Velocity = directionToPosition * characterMovementComponent->GetMaxSpeed();
    UE_LOG(LogTemp, Display, TEXT("NEW TARGET: %s"), *characterMovementComponent->Velocity.ToString());

    float distanceToPosition = FVector::Distance(pawnLocation, targetPosition);
    if (distanceToPosition <= acceptanceRadius)
    {
        characterMovementComponent->BrakingDecelerationFlying = initialBrakingDeceleration;
        characterMovementComponent->MaxFlySpeed = initialSpeed;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
