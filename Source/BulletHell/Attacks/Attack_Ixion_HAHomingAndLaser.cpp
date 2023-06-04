// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_HAHomingAndLaser.h"
#include "Kismet/GameplayStatics.h"
#include "IxionAIController.h"
#include "Projectile/Projectile.h"

void UAttack_Ixion_HAHomingAndLaser::Start()
{
	GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_HAHomingAndLaser::FireBullets, bulletsStartTime, true);
}

void UAttack_Ixion_HAHomingAndLaser::Finish()
{
	AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
	ixion->FinishAttack(false);
}

void UAttack_Ixion_HAHomingAndLaser::FireBullets()
{
    float anglePerBullet = bulletsConeAngle / bulletsStartPoints;
    float halfRange = bulletsConeAngle / 2;

    FVector location = GetPawn()->GetActorLocation();
    FRotator rotation = GetPawn()->GetActorRotation();

    for (int i = 0; i < 2; ++i) {
        float sideRotation = i == 0 ? 90 : -90;
        for (int j = 0; j < bulletsPerStartPoint; ++j) {
            FTransform transform = FTransform(rotation + FRotator(-halfRange + anglePerBullet * (j + 1), sideRotation, 0), location);

            AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
            if (projectile) {
              /*  FVector velocityOutwards = pointRotation.Vector();
                FVector finalVelocity = FVector::CrossProduct(velocityOutwards, rotationTowardsPlayer.Vector());*/

                projectile->SetSpeed(bulletsSpeed);
                UGameplayStatics::FinishSpawningActor(projectile, transform);
                //projectile->SetVelocity(finalVelocity * bulletSpeedVortex);
                projectile->SetOwner(GetOwner());
                projectile->SetLifeSpan(bulletsDuration);
                projectile->SetHoming(GetOwner()->GetPlayerPawn());
                projectile->SetHomingMagnitude(bulletsHomingMagnitude);
            }
        }

    }
}
