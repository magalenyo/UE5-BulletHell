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

    FVector parentLocation = controller->GetPawn()->GetActorLocation();
    // UNavigationSystem* NavSys = World->GetNavigationSystem();
    FVector randomLocation = UNavigationSystemV1::GetRandomPointInNavigableRadius(controller, parentLocation, radius);

    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard) {
        return EBTNodeResult::Failed;
    }

    blackboard->SetValueAsVector(GetSelectedBlackboardKey(), randomLocation + offset);

    return EBTNodeResult::Succeeded;
}
