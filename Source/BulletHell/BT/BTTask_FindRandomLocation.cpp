// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation() 
{
    NodeName = TEXT("Find random location");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* controller = OwnerComp.GetAIOwner();
    if (!controller) {
        return EBTNodeResult::Failed;
    }

    UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(controller->GetWorld());
    FNavLocation randomLocation;
    bool found = navSystem->GetRandomPointInNavigableRadius(
        controller->GetPawn()->GetActorLocation(), 
        radius, 
        randomLocation
    );
    
    if (!found) {
        return EBTNodeResult::Failed;
    }

    UE_LOG(LogTemp, Display, TEXT("RANDOM POSITION %s"), *randomLocation.Location.ToString());

    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard) {
        return EBTNodeResult::Failed;
    }

    blackboard->SetValueAsVector(GetSelectedBlackboardKey(), randomLocation.Location + offset);

    return EBTNodeResult::Succeeded;
}
