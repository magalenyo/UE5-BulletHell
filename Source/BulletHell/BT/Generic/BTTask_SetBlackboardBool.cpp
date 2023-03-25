// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetBlackboardBool.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBlackboardBool::UBTTask_SetBlackboardBool()
{
    NodeName = TEXT("Set Blackboard Bool"); 
}

EBTNodeResult::Type UBTTask_SetBlackboardBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard) {
        return EBTNodeResult::Failed;
    }

    blackboard->SetValueAsBool(GetSelectedBlackboardKey(), value);

    return EBTNodeResult::Succeeded;
}
