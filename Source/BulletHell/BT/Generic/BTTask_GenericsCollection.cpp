// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Generic/BTTask_GenericsCollection.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_AddPositionTo::UBTTask_AddPositionTo()
{
    NodeName = TEXT("Add Offset Position"); 
}

EBTNodeResult::Type UBTTask_AddPositionTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard) {
        return EBTNodeResult::Failed;
    }

    AAIController* controller = OwnerComp.GetAIOwner();
    APawn* pawn = controller->GetPawn();

    blackboard->SetValueAsVector(GetSelectedBlackboardKey(), pawn->GetActorLocation() + offset);

    return EBTNodeResult::Succeeded;
}
