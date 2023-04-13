// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Generic/BTDecorator_BBCompareInt.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_BBCompareInt::UBTDecorator_BBCompareInt()
{
    NodeName = TEXT("Blackboard Compare Int");
}

bool UBTDecorator_BBCompareInt::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const 
{
    return value == OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());
}

