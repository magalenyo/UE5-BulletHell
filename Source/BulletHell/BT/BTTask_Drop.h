// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Drop.generated.h"

class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class BULLETHELL_API UBTTask_Drop : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Drop();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere)
    float speed = 600;

	UPROPERTY(EditAnywhere)
    float acceptanceRadius = 20;

	UPROPERTY(EditAnywhere)
	float dropDistance = 2000;

	float initialBrakingDeceleration = 0;
    float maxBrakingDeceleration = 2000;
    float initialSpeed;

	FVector targetPosition = FVector::ZeroVector;

	UCharacterMovementComponent* characterMovementComponent;

    // UCharacterMovementComponent* GetCharacterMovement(const UBehaviorTreeComponent& OwnerComp) const;
};
