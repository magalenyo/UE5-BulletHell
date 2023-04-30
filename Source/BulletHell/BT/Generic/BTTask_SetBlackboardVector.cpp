// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Generic/BTTask_SetBlackboardVector.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBlackboardVector::UBTTask_SetBlackboardVector()
{
    NodeName = TEXT("Set Blackboard Vector"); 
}

EBTNodeResult::Type UBTTask_SetBlackboardVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard) {
        return EBTNodeResult::Failed;
    }

    blackboard->SetValueAsVector(GetSelectedBlackboardKey(), value);

    return EBTNodeResult::Succeeded;
}

