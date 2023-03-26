// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Generic/BTDecorator_RandomChance.h"

UBTDecorator_RandomChance::UBTDecorator_RandomChance()
{
    NodeName = "Random Chance";
}

bool UBTDecorator_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const 
{
    return FMath::FRand() <= chance;
}

