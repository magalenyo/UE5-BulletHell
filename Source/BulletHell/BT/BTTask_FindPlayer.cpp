// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_FindPlayer::UBTTask_FindPlayer() 
{
    NodeName = TEXT("Find player");
}

EBTNodeResult::Type UBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (!playerPawn) {
        return EBTNodeResult::Failed;
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), playerPawn);

    return EBTNodeResult::Succeeded;
}
