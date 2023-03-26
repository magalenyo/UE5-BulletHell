// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyTo.generated.h"

class UCharacterMovementComponent;

UCLASS()
class BULLETHELL_API UBTTask_FlyTo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FlyTo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector destinationKey;

    UPROPERTY(EditAnywhere)
    float speed = 500;

    UPROPERTY(EditAnywhere)
    float acceptanceRadius = 40;

    UPROPERTY(EditAnywhere)
    float flightHeight = 500;

private:
    float initialBrakingDeceleration = 0;
    float maxBrakingDeceleration = 2000;
    UCharacterMovementComponent* characterMovementComponent;

    FVector GetDestination(const UBehaviorTreeComponent& OwnerComp) const;

    UCharacterMovementComponent* GetCharacterMovement(const UBehaviorTreeComponent& OwnerComp) const;
};
