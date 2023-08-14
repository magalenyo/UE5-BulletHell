// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTL_RandomGenerics.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API UBTService_RandomChanceMultiple : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_RandomChanceMultiple();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};

UCLASS()
class BULLETHELL_API UBTDecorator_RandomChanceMultiple : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_RandomChanceMultiple();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float chance = 0.5f;
};
