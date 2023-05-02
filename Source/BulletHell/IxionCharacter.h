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
	virtual void HandleDeath();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* projectileSpawnPointTopPhase2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* projectileSpawnPointBottomPhase2;

	int phase = 1;

	void ChangePhase(int newPhase);
};
