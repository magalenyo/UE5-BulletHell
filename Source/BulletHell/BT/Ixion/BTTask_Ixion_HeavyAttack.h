// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Ixion_HeavyAttack.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UBTTask_Ixion_HeavyAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Ixion_HeavyAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
