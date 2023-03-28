// Fill out your copyright notice in the Description page of Project Settings.


#include "IxionAIController.h"
#include "Projectile/Projectile.h"
#include "ShooterCharacter.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "BT/Ixion/BTTask_Ixion_BasicAttack.h"


void AIxionAIController::BeginPlay()
{
    Super::BeginPlay();
}


void AIxionAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (isAttacking) {
        LookAtPlayer();
        // BasicAttack();
    }
}

void AIxionAIController::StartBasicAttack()
{
    isAttacking = true;

    GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BasicAttack, .05, true);
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

    float randomX = FMath::RandRange(-radiusOffset, radiusOffset);
    float randomY = FMath::RandRange(-radiusOffset, radiusOffset);
    FRotator randomRotation = LookAt(location, playerPawn->GetActorLocation() + FVector(randomX, randomY, -heightOffset));

	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, location, randomRotation);
	projectile->SetOwner(this);

    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);

    currentBulletsBasicAttack++;
    if (currentBulletsBasicAttack == bulletsBasicAttack){
        isAttacking = false;
        onBasicAttackFinishedDelegate.ExecuteIfBound();
        currentBulletsBasicAttack = 0;
    }
    else{
        float nextTime = FMath::RandRange(.05f, .15f);
        GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BasicAttack, nextTime, true);
    }
}
