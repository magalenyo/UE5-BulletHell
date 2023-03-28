// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Ixion/BTTask_Ixion_BasicAttack.h"
#include "IxionAIController.h"


UBTTask_Ixion_BasicAttack::UBTTask_Ixion_BasicAttack()
{
    NodeName = TEXT("Ixion Basic Attack");
    bNotifyTick = true;
    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Ixion_BasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    AIxionAIController* character = Cast<AIxionAIController>(OwnerComp.GetAIOwner());

    if (!character) {
        EBTNodeResult::Failed;
    }

    isAttacking = true;
    character->StartBasicAttack();
    character->onBasicAttackFinishedDelegate.BindLambda([&]()
    {
        isAttacking = false;
    });

    return EBTNodeResult::InProgress;
}

void UBTTask_Ixion_BasicAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    if (!isAttacking) {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
