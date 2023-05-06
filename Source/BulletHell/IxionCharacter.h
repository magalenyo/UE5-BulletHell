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
	
public:
	AIxionCharacter();

	const USceneComponent* GetProjectileSpawnPointTop() const;
	const USceneComponent* GetProjectileSpawnPointBottom() const;
	
protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* projectileSpawnPointTopPhase2;

	UPROPERTY(EditAnywhere)
	USceneComponent* projectileSpawnPointBottomPhase2;

	virtual void HandleDeath();

private:

	int phase = 1;

	void ChangePhase(int newPhase);
};
