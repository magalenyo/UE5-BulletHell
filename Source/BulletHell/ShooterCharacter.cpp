// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "BulletHellGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;
	
	gun = GetWorld()->SpawnActor<AGun>(gunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));		// This is used whenever we are not attaching from constructor
	gun->SetOwner(this);		// Needed for multiplayer and damage
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AShooterCharacter::JumpUp);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AShooterCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AShooterCharacter::StopSprint);

}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float damageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damageToApply = FMath::Min(health, damageToApply);
	health -= damageToApply;
	UE_LOG(LogTemp, Display, TEXT("Health left:%f"), health);

	if (IsDead()) {
		// ABulletHellGameModeBase* gameMode = GetWorld()->GetAuthGameMode<ABulletHellGameModeBase>();
		// if (gameMode != nullptr) {
		// 	gameMode->PawnKilled(this);
		// }
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return damageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return health / maxHealth;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue * movementSpeed * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue * movementSpeed * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue * mouseRotationSpeedVertical * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * rotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * mouseRotationSpeedHorizontal * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * rotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::JumpUp()
{
	Jump();
}

void AShooterCharacter::Shoot()
{
	if (isSprinting) {
		return;
	}
	gun->PullTrigger();
}

void AShooterCharacter::StartSprint()
{
	if (!GetCharacterMovement()->IsFalling()) {
		GetCharacterMovement()->MaxWalkSpeed *= sprintMultiplier;
		GetCharacterMovement()->JumpZVelocity *= jumpMultiplier;
		isSprinting = true;
	}

}

void AShooterCharacter::StopSprint()
{
	if (isSprinting) {
		GetCharacterMovement()->MaxWalkSpeed /= sprintMultiplier;
		GetCharacterMovement()->JumpZVelocity /= jumpMultiplier;
		isSprinting = false;
	}

}
