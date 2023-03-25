// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBackboardValue::UBTTask_ClearBackboardValue()
{
    // The name in the blackboard 
    NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    return EBTNodeResult::Succeeded;
}
