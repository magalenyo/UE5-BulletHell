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

    projectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn point"));
	projectileSpawnPoint->SetupAttachment(GetMesh());
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

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float damageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damageToApply = FMath::Min(health, damageToApply);
	health -= damageToApply;

	HandleDeath();

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

const USceneComponent* AShooterCharacter::GetProjectileSpawnPoint() const
{
	return projectileSpawnPoint;
}

const float AShooterCharacter::GetMovementSpeed() const
{
	return GetCharacterMovement()->Velocity.Size();
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

void AShooterCharacter::HandleDeath()
{
	if (IsDead()) {
		// ABulletHellGameModeBase* gameMode = GetWorld()->GetAuthGameMode<ABulletHellGameModeBase>();
		// if (gameMode != nullptr) {
		// 	gameMode->PawnKilled(this);
		// }
		DisableCharacter();
	}
}

void AShooterCharacter::DisableCharacter()
{
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
