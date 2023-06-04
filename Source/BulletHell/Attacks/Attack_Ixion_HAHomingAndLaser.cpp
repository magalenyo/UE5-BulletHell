// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_HAHomingAndLaser.h"
#include "Kismet/GameplayStatics.h"
#include "IxionAIController.h"
#include "Projectile/Projectile.h"

void UAttack_Ixion_HAHomingAndLaser::Start()
{
    if (!homingProjectileClass) {
        Finish();
    }

	GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_HAHomingAndLaser::FireBullets, bulletsStartTime, true);
}

void UAttack_Ixion_HAHomingAndLaser::Finish()
{
    currentWave = 0;

	AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
	ixion->FinishAttack(false);
}

void UAttack_Ixion_HAHomingAndLaser::FireBullets()
{
    GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);

    float halfRange = bulletsConeAngle / 2;

    FVector location = GetPawn()->GetActorLocation();
    FRotator rotation = GetPawn()->GetActorRotation();

    for (int i = 0; i < 2; ++i) {
        float sideRotation = i == 0 ? 90 : -90;
        for (int j = 0; j < bulletsPerWave; ++j) {
            float randomAngle = FMath::RandRange(-halfRange, halfRange);
            FTransform transform = FTransform(rotation + FRotator(FMath::RandRange(-halfRange, halfRange), sideRotation, 0), location);

            AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(homingProjectileClass, transform);
            if (projectile) {
                projectile->SetSpeed(FMath::RandRange(bulletsSpeed, bulletsSpeed * 1.1f));
                UGameplayStatics::FinishSpawningActor(projectile, transform);
                projectile->SetOwner(GetOwner());
                projectile->SetLifeSpan(bulletsDuration);
                projectile->SetHoming(GetOwner()->GetPlayerPawn());
                projectile->SetHomingMagnitude(bulletsHomingMagnitude);
            }
        }
    }

    currentWave++;
    if (currentWave == waves) {
        Finish();
    }
    else {
        float nextTime = FMath::RandRange(bulletsMinTimeWave, bulletsMaxTimeWave);
        GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_HAHomingAndLaser::FireBullets, nextTime, true);
    }

}
