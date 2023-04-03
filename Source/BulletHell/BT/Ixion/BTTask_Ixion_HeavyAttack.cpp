// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Ixion/BTTask_Ixion_HeavyAttack.h"


UBTTask_Ixion_HeavyAttack::UBTTask_Ixion_HeavyAttack()
{
    NodeName = TEXT("Ixion Heavy Attack");
    bNotifyTick = true;
    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Ixion_HeavyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AIxionAIController* character = Cast<AIxionAIController>(OwnerComp.GetAIOwner());

    if (!character) {
        EBTNodeResult::Failed;
    }

    isAttacking = true;
    character->StartHeavyAttack(attack);
    character->onHeavyAttackFinishedDelegate.BindLambda([&]()
    {
        isAttacking = false;
    });

    return EBTNodeResult::InProgress;
}

void UBTTask_Ixion_HeavyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    if (!isAttacking) {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
