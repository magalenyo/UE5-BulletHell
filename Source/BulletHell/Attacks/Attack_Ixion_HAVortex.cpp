// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_HAVortex.h"
#include "Projectile/Projectile.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h" 
#include "IxionAIController.h"

void UAttack_Ixion_HAVortex::Start()
{
    projectilesVortex.resize(startPointsPerWaveVortex * wavesPerPointVortex);

    APawn* playerPawn = GetOwner()->GetPlayerPawn();

    if (!playerPawn) {
        return;
    }

    targetPositionVortex = playerPawn->GetActorLocation();
    GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_HAVortex::HAVortex, .05, true);
}

void UAttack_Ixion_HAVortex::Finish()
{
    currentWaveVortex = 0;
    currentWaveVortexReposition = 0;

    projectilesVortex.clear();
    
    GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(retargetWaveVortexTimerHandle);

    AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
    if (ixion) {
        ixion->FinishAttack();
    }
    ixion->FinishAttack(false);
}

void UAttack_Ixion_HAVortex::HAVortex() 
{
    AShooterCharacter* character = Cast<AShooterCharacter>(GetOwner()->GetPawn());

    if (!character) {
        return;
    }

    const USceneComponent* projectileSpawnPoint = character->GetProjectileSpawnPoint();

    if (!projectileSpawnPoint) {
        return;
    }

    FVector location = projectileSpawnPoint->GetComponentLocation();
    FRotator rotationTowardsPlayer = GetOwner()->LookAt(targetPositionVortex, location);

    float angleToRotate = 360.0f / startPointsPerWaveVortex;
    float angleOffsetPerWave = rotationSpeedVortex / durationVortex;
    float angleExtraPerWave = 360 / wavesPerPointVortex;

    for (int i = 0; i < startPointsPerWaveVortex; ++i) {
        float extra = angleExtraPerWave * currentWaveVortex * angleOffsetPerWave;
        FRotator pointRotation = rotationTowardsPlayer + FRotator((angleToRotate * i) + extra, 90, 0);

        FVector bulletLocation = location;
        FTransform transform = FTransform(pointRotation, bulletLocation);

        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
        if (projectile) {
            FVector velocityOutwards = pointRotation.Vector();
            FVector finalVelocity = FVector::CrossProduct(velocityOutwards, rotationTowardsPlayer.Vector());

            projectile->SetSpeed(speedVortexAttack);
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetVelocity(finalVelocity * speedVortexAttack);
            projectile->SetOwner(GetOwner());
        }
        projectilesVortex[i + (currentWaveVortex * startPointsPerWaveVortex)] = projectile;
    }

    GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);

    float timePerWave = durationVortex / wavesPerPointVortex;
    float timeToReposition = timePerWave * currentWaveVortex;

    if (currentWaveVortex == 0) {
        GetWorld()->GetTimerManager().SetTimer(retargetWaveVortexTimerHandle, this, &UAttack_Ixion_HAVortex::RepositionVortexProjectiles, 0.05, false);
    }

    currentWaveVortex++;
    if (currentWaveVortex != wavesPerPointVortex) {
        float nextTime = durationVortex / wavesPerPointVortex;
        GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_HAVortex::HAVortex, nextTime, true);
    }
}

void UAttack_Ixion_HAVortex::RepositionVortexProjectiles() 
{
    if (currentWaveVortexReposition == wavesPerPointVortex) {
        GetWorld()->GetTimerManager().ClearTimer(retargetWaveVortexTimerHandle);
        Finish();
        return;
    }

    for (int i = (currentWaveVortexReposition * startPointsPerWaveVortex); i < ((currentWaveVortexReposition + 1) * startPointsPerWaveVortex); ++i)
    {
        AProjectile* projectile = projectilesVortex[i];
        if (projectile) {
            FVector directionToPlayer = (targetPositionVortex - projectile->GetActorLocation()).GetSafeNormal();
            projectile->SetVelocity(directionToPlayer * projectile->GetProjectileSpeed());
        }
    }
    float timePerWave = durationVortex / wavesPerPointVortex;
    float timeToReposition = timePerWave * 2;
    currentWaveVortexReposition++;

    GetWorld()->GetTimerManager().ClearTimer(retargetWaveVortexTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(retargetWaveVortexTimerHandle, this, &UAttack_Ixion_HAVortex::RepositionVortexProjectiles, timeToReposition, false);
}
