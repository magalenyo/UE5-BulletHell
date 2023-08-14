// Fill out your copyright notice in the Description page of Project Settings.

#include "BT/Generic/BTL_RandomGenerics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_RandomChanceMultiple::UBTService_RandomChanceMultiple()
{
	NodeName = TEXT("Random Chance Multiple");
}

void UBTService_RandomChanceMultiple::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), FMath::FRand());
}

UBTDecorator_RandomChanceMultiple::UBTDecorator_RandomChanceMultiple()
{
	NodeName = TEXT("Random Chance Multiple");
}

bool UBTDecorator_RandomChanceMultiple::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey()) <= chance;
}
