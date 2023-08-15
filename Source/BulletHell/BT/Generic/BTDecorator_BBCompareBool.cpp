// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Generic/BTDecorator_BBCompareBool.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_BBCompareBool::UBTDecorator_BBCompareBool()
{
    NodeName = TEXT("Blackboard Compare Bool");
}

bool UBTDecorator_BBCompareBool::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return value == OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
}
