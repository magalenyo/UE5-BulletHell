// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterAIController.h"
#include "Attack.generated.h"

/**
 * 
 */
// UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class BULLETHELL_API UAttack : public UObject
{
	GENERATED_BODY()

public:
	UAttack();
	UAttack(AShooterAIController* newOwner);
	~UAttack();

	virtual void Start() { };
	virtual void Tick(float DeltaTime) { };
	virtual void Finish() { };

	void SetOwner(AShooterAIController* newOwner);

protected:

	AShooterAIController* GetOwner();
	APawn* GetPawn();

private:
	AShooterAIController* owner;
};
