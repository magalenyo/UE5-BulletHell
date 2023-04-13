// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Generic/BTDecorator_BBCompareFloat.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_BBCompareFloat::UBTDecorator_BBCompareFloat()
{
    NodeName = TEXT("Blackboard Compare Float");
}

bool UBTDecorator_BBCompareFloat::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const 
{
    return value == OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());
}

