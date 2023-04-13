// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerCharacter.h"

// Called to bind functionality to input
void AShooterPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterPlayerCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterPlayerCharacter::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterPlayerCharacter::LookRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AShooterPlayerCharacter::JumpUp);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterPlayerCharacter::Shoot);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AShooterPlayerCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AShooterPlayerCharacter::StopSprint);
}

void AShooterPlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue * movementSpeed * GetWorld()->GetDeltaSeconds());
}

void AShooterPlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue * movementSpeed * GetWorld()->GetDeltaSeconds());
}

void AShooterPlayerCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue * mouseRotationSpeedVertical * GetWorld()->GetDeltaSeconds());
}

void AShooterPlayerCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * rotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterPlayerCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * mouseRotationSpeedHorizontal * GetWorld()->GetDeltaSeconds());
}

void AShooterPlayerCharacter::LookRightRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * rotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterPlayerCharacter::JumpUp()
{
	Jump();
}
