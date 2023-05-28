// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindRandomLocationRadius.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocationRadius::UBTTask_FindRandomLocationRadius()
{
    NodeName = TEXT("Find random location in radius");
}

EBTNodeResult::Type UBTTask_FindRandomLocationRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    FVector randomLocation = FVector(FMath::RandPointInCircle(radius).GetSafeNormal() * radius, 0);
    randomLocation.Z = offset.Z;

    UE_LOG(LogTemp, Display, TEXT("RANDOM POSITION %s"), *randomLocation.ToString());

    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard) {
        return EBTNodeResult::Failed;
    }

    blackboard->SetValueAsVector(GetSelectedBlackboardKey(), randomLocation + offset);

    return EBTNodeResult::Succeeded;
}
