// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IxionAIController.h"
#include "BTTask_Ixion_BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UBTTask_Ixion_BasicAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Ixion_BasicAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool isAttacking = false;

	UPROPERTY(EditAnywhere)
	EIxionBasicAttack attack;
};
