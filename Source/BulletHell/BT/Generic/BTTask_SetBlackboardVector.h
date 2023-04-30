// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetBlackboardVector.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UBTTask_SetBlackboardVector : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_SetBlackboardVector();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	FVector value;
};
