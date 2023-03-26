// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_RandomChance.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UBTDecorator_RandomChance : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_RandomChance();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
    float chance = 0.5f;
};
