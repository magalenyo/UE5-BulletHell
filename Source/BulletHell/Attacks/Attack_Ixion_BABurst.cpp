// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_BABurst.h"
#include "ShooterCharacter.h"
#include "Projectile/Projectile.h"
#include "IxionAIController.h"
#include "Kismet/GameplayStatics.h" 
#include "Kismet/KismetMathLibrary.h"

void UAttack_Ixion_BABurst::Start()
{
    projectilesBurst.resize(bulletsPerWaveBurstAttack * wavesBurstAttack);
    GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_BABurst::BABurst, .05, true);
}

void UAttack_Ixion_BABurst::Finish()
{
    currentWaveBurst = 0;
    projectilesBurst.clear();
    AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
    if (ixion) {
        ixion->FinishAttack();
    }
}

void UAttack_Ixion_BABurst::BABurst()
{
    AShooterCharacter* character = Cast<AShooterCharacter>(GetPawn());

    if (!character) {
        return;
    }

    const USceneComponent* projectileSpawnPoint = character->GetProjectileSpawnPoint();

    if (!projectileSpawnPoint) {
        return;
    }

    APawn* playerPawn = GetOwner()->GetPlayerPawn();

    if (!playerPawn) {
        return;
    }

    FVector location = projectileSpawnPoint->GetComponentLocation();

    FVector directionTowardsPlayer = (playerPawn->GetActorLocation() - location).GetSafeNormal();

    for (int i = 0; i < bulletsPerWaveBurstAttack; ++i) {
        FVector direction = UKismetMathLibrary::RandomUnitVectorInConeInRadians(directionTowardsPlayer, UKismetMathLibrary::DegreesToRadians(angleBurstAttack));
        FTransform transform = FTransform(direction.Rotation(), location);
        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
        if (projectile)
        {
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetSpeed(speedBurstAttack);
            projectile->SetOwner(GetOwner());

            projectilesBurst[i + (currentWaveBurst * bulletsPerWaveBurstAttack)] = projectile;
        }
    }

    GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);

	currentWaveBurst++;

    if (currentWaveBurst == wavesBurstAttack) {
        GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, [this]() {
            APawn* playerPawn = GetOwner()->GetPlayerPawn();

            if (!playerPawn) {
                return;
            }

            FVector playerLocation = playerPawn->GetActorLocation();
            for (AProjectile* projectile : projectilesBurst)
            {
                if (projectile) {
                    // Try to "retarget" the parallel vector from player to projectileSpawnPoint
                    // FVector directionToPlayer = (playerLocation - projectile->GetActorLocation()).GetSafeNormal();
                    // projectile->SetVelocity(directionToPlayer * projectile->GetProjectileSpeed());
                }
            }
            Finish();
        }, delayToHomeBurstAttack, false);
    }
    else {
        float nextTime = durationBurstAttack / wavesBurstAttack;
        GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_BABurst::BABurst, nextTime, true);
    }
}
