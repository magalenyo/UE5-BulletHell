// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_BBCompareInt.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UBTDecorator_BBCompareInt : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_BBCompareInt();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
    int value;
};
