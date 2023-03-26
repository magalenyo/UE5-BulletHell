// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Ixion/BTTask_Ixion_BasicAttack.h"


UBTTask_Ixion_BasicAttack::UBTTask_Ixion_BasicAttack()
{
    NodeName = TEXT("Ixion Basic Attack"); 
}

EBTNodeResult::Type UBTTask_Ixion_BasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::Succeeded;
}
