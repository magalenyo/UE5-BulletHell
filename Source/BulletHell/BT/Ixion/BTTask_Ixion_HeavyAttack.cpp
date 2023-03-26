// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Ixion/BTTask_Ixion_HeavyAttack.h"


UBTTask_Ixion_HeavyAttack::UBTTask_Ixion_HeavyAttack()
{
    NodeName = TEXT("Ixion Heavy Attack");
}

EBTNodeResult::Type UBTTask_Ixion_HeavyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeREsult::Succeded;
}
