// Fill out your copyright notice in the Description page of Project Settings.

#include "Attacks/Attack_Ixion_BAMachineGun.h"
#include "Projectile/Projectile.h"
#include "Kismet/GameplayStatics.h" 
#include "ShooterCharacter.h"
#include "IxionAIController.h"

void UAttack_Ixion_BAMachineGun::Start() 
{
    GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_BAMachineGun::BAMachineGun, .05, true);
}

void UAttack_Ixion_BAMachineGun::Finish()
{
    currentBulletsMachineGun = 0;
    GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);
    AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
    if (ixion) {
        ixion->FinishAttack();
    }
}

void UAttack_Ixion_BAMachineGun::BAMachineGun()
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

    float randomX = FMath::RandRange(-radiusOffset, radiusOffset);
    float randomY = FMath::RandRange(-radiusOffset, radiusOffset);
    float randomZ = FMath::RandRange(-radiusOffset, radiusOffset);
    FVector randomTargetPosition = playerPawn->GetActorLocation() + FVector(randomX, randomY, -heightOffset + randomZ);
    FRotator randomRotation = GetOwner()->LookAt(location, randomTargetPosition);

    FTransform transform = FTransform(randomRotation, location);
    AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
    if (projectile)
    {
        UGameplayStatics::FinishSpawningActor(projectile, transform);
        
        projectile->SetPredictionSpeed(randomTargetPosition, playerPawn->GetVelocity(), predictionSpeedMitigationFactorMachineGun);
        projectile->SetOwner(GetOwner());
    }

    GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);

    currentBulletsMachineGun++;
    if (currentBulletsMachineGun == bulletsBasicAttack){
        Finish();
    }
    else{
        float nextTime = FMath::RandRange(nextAttackMinTimeMachineGun, nextAttackMaxTimeMachineGun);
        GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_BAMachineGun::BAMachineGun, nextTime, true);
    }
}
