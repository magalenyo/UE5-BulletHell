// Fill out your copyright notice in the Description page of Project Settings.


#include "IxionAIController.h"
#include "Projectile/Projectile.h"
#include "ShooterCharacter.h"
#include "TimerManager.h"
#include "BT/Ixion/BTTask_Ixion_BasicAttack.h"


void AIxionAIController::BeginPlay()
{
    Super::BeginPlay();
}


void AIxionAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (isAttacking) {
        BasicAttack();
    }
}

void AIxionAIController::StartBasicAttack()
{
    isAttacking = true;

    GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BasicAttack, 1.0, true);
}

void AIxionAIController::BasicAttack()
{
    LookAtPlayer();

    AShooterCharacter* character = Cast<AShooterCharacter>(GetPawn());

    if (!character) {
        return;
    }

    const USceneComponent* projectileSpawnPoint = character->GetProjectileSpawnPoint();

    if (!projectileSpawnPoint) {
        return;
    }

    FVector location = projectileSpawnPoint->GetComponentLocation();
	FRotator rotation = projectileSpawnPoint->GetComponentRotation();

	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, location, GetPawn()->GetActorRotation());
	projectile->SetOwner(this);
    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);
    toDelete++;
    if (toDelete == 30){
        isAttacking = false;
        onBasicAttackFinishedDelegate.ExecuteIfBound();
        toDelete = 0;
    }
}
