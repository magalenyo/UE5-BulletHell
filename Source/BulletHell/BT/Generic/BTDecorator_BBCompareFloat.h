// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_BBCompareFloat.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UBTDecorator_BBCompareFloat : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_BBCompareFloat();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
    float value;
};
