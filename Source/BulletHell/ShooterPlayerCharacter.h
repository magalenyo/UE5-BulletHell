// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacter.h"
#include "ShooterPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API AShooterPlayerCharacter : public AShooterCharacter
{
	GENERATED_BODY()
	
public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere)
	float rotationRate = 40;

	UPROPERTY(EditAnywhere)
	float mouseRotationSpeedHorizontal = 20;

	UPROPERTY(EditAnywhere)
	float mouseRotationSpeedVertical = 15;

	void MoveForward(float AxisValue);	// No additional methods needed for controller input since it already is suported for both (it takes a rate instead of a value)
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookUpRate(float AxisValue);	// Used for controller input only
	void LookRight(float AxisValue);
	void LookRightRate(float AxisValue);
	void JumpUp();

};
