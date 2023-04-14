// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacter.h"
#include "IxionCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API AIxionCharacter : public AShooterCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void HandleDeath();

private:
	int phase = 1;

	void ChangePhase(int newPhase);
};
