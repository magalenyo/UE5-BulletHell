// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	bool IsDead() const;

	const float GetFlyOffset();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float flyOffset = 500;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	APawn* playerPawn;

};
