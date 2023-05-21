// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_HAShockwaveBullets.h"
#include "AttacksBehaviors/AttackBehavior_Ixion_BAShockwave.h"
#include "Projectile/Projectile.h"
#include "Kismet/GameplayStatics.h" 
#include "IxionAIController.h"
#include "ShooterCharacter.h"
#include "IxionCharacter.h"

void UAttack_Ixion_HAShockwaveBullets::Start()
{
    AIxionCharacter* character = Cast<AIxionCharacter>(GetOwner()->GetPawn());
    if (!character) {
        Finish();
        return;
    }

    projectileSpawnPointTop = character->GetProjectileSpawnPointTop();
    projectileSpawnPointBottom = character->GetProjectileSpawnPointBottom();

    GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UAttack_Ixion_HAShockwaveBullets::Finish, duration - 0.02f);
    GetWorld()->GetTimerManager().SetTimer(shockwaveFireRateTimerHandle, this, &UAttack_Ixion_HAShockwaveBullets::FireShockwave, duration / shockwaveNumber, true, 0.0f);
    GetWorld()->GetTimerManager().SetTimer(bulletsFireRateTimerHandle, this, &UAttack_Ixion_HAShockwaveBullets::FireBulletsWave, duration / shockwaveNumber, true, bulletsWaveDelay);
    GetWorld()->GetTimerManager().SetTimer(burstFireRateTimerHandle, this, &UAttack_Ixion_HAShockwaveBullets::FireBurst, duration / shockwaveNumber, true, burstWaveDelay);
}

void UAttack_Ixion_HAShockwaveBullets::Finish()
{
    GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(shockwaveFireRateTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(bulletsFireRateTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(bulletsSubwaveFireRateTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(burstFireRateTimerHandle);

    AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
    if (ixion) {
        ixion->FinishAttack();
    }
    ixion->FinishAttack(false);
}

void UAttack_Ixion_HAShockwaveBullets::FireShockwave()
{
    if (shockwaveClass) {
        AAttackBehavior_Ixion_BAShockwave* shockwave = GetWorld()->SpawnActor<AAttackBehavior_Ixion_BAShockwave>(shockwaveClass);
        if (shockwave) {
            shockwave->SetLifeSpan(shockwaveDuration);
            shockwave->SetSpeed(shockwaveSpeed);
            shockwave->SetScaleGrow(true);
            shockwave->SetZGrow(true);
            shockwave->SetZGrowSpeed(shockwaveZSpeed);
        }
    }
}

void UAttack_Ixion_HAShockwaveBullets::FireBulletsWave()
{
    AShooterCharacter* character = Cast<AShooterCharacter>(GetOwner()->GetPawn());

    if (!character) {
        return;
    }

    if (!projectileSpawnPointBottom) {
        return;
    }

    FVector location = projectileSpawnPointBottom->GetComponentLocation();

    float angleToRotate = 360.0f / bulletsStartPoints;

    for (int i = 0; i < bulletsStartPoints; ++i) {
        FRotator pointRotation = FRotator(0, (angleToRotate * i), 0);

        FVector bulletLocation = location;
        FTransform transform = FTransform(pointRotation, bulletLocation);

        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
        if (projectile) {
            FVector velocityOutwards = pointRotation.Vector();

            projectile->SetSpeed(bulletsSpeed);
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetOwner(GetOwner());
        }
    }

    float nextTime = FMath::RandRange(bulletsSubwaveMin, bulletsSubwaveMax);

    GetWorld()->GetTimerManager().SetTimer(bulletsSubwaveFireRateTimerHandle, this, &UAttack_Ixion_HAShockwaveBullets::FireBulletsSubwave, nextTime, false);
}

void UAttack_Ixion_HAShockwaveBullets::FireBulletsSubwave()
{
    GetWorld()->GetTimerManager().ClearTimer(bulletsSubwaveFireRateTimerHandle);

    AShooterCharacter* character = Cast<AShooterCharacter>(GetOwner()->GetPawn());

    if (!character) {
        return;
    }

    if (!projectileSpawnPointBottom) {
        return;
    }

    FVector location = projectileSpawnPointBottom->GetComponentLocation();

    float angleToRotate = 360.0f / bulletsStartPoints;

    for (int i = 0; i < bulletsStartPoints; ++i) {

        if (FMath::RandRange(0.0f, 1.0f) > bulletsSubwaveChance) {
            continue;
        }

        FRotator pointRotation = FRotator(0, (angleToRotate * i) + FMath::RandRange(0.0f, 15.0f), 0);

        FVector bulletLocation = location;
        FTransform transform = FTransform(pointRotation, bulletLocation);

        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
        if (projectile) {
            FVector velocityOutwards = pointRotation.Vector();

            projectile->SetSpeed(bulletsSpeed);
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetOwner(GetOwner());
        }
    }
}

void UAttack_Ixion_HAShockwaveBullets::FireBurst()
{
    AShooterCharacter* character = Cast<AShooterCharacter>(GetOwner()->GetPawn());

    if (!character) {
        return;
    }

    if (!projectileSpawnPointTop) {
        return;
    }

    FVector location = projectileSpawnPointTop->GetComponentLocation();
	FRotator rotation = projectileSpawnPointTop->GetComponentRotation();

    for (int i = 0; i < burstBullets; ++i) {
        FVector randomPosition = FMath::VRand() * 500 + location;
        FTransform transform = FTransform(GetOwner()->LookAt(randomPosition), location);
        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
        if (projectile)
        {
            projectile->SetSpeed(burstSpeed);
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetOwner(GetOwner());
            projectile->SetGravity(burstGravity);
            if (burstDecelerationCurve) {
                projectile->SetDecelerationCurve(burstDecelerationCurve);
            }
        }
    }
}
