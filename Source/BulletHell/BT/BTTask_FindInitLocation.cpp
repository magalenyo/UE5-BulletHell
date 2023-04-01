// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindInitLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterAIController.h"


UBTTask_FindInitLocation::UBTTask_FindInitLocation() 
{
    NodeName = TEXT("Find initial position");
}

EBTNodeResult::Type UBTTask_FindInitLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AShooterAIController* controller = Cast<AShooterAIController>(OwnerComp.GetAIOwner());
    if (!controller) {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard) {
        return EBTNodeResult::Failed;
    }
    
    FVector parentLocation = controller->GetPawn()->GetActorLocation();
    FVector targetLocation = FVector(parentLocation.X, parentLocation.Y, parentLocation.Z + controller->GetFlyOffset());
    blackboard->SetValueAsVector(GetSelectedBlackboardKey(), targetLocation);

    return EBTNodeResult::Succeeded;
}
