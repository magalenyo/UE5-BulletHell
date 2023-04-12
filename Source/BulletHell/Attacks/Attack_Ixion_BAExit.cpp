// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_BAExit.h"
#include "Projectile/Projectile.h"
#include "Kismet/GameplayStatics.h" 
#include "ShooterCharacter.h"
#include "IxionAIController.h"

void UAttack_Ixion_BAExit::Start()
{
    GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_BAExit::BAExit, .05, true);
}

void UAttack_Ixion_BAExit::Finish()
{
    GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);
    AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
    if (ixion) {
        ixion->FinishAttack();
    }
}

void UAttack_Ixion_BAExit::BAExit()
{
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

    for (int i = 0; i < bulletsExitAttack; ++i) {
        FVector randomPosition = FMath::VRand() * 500 + location;
        FTransform transform = FTransform(GetOwner()->LookAt(randomPosition), location);
        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
        if (projectile)
        {
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetOwner(GetOwner());
            projectile->SetLifeSpan(lifeSpanExitAttack);
            projectile->SetSpeed(speedExitAttack);
            if (useDecelerationCurveExitAttack && decelerationCurveExitAttack) {
                projectile->SetDecelerationCurve(decelerationCurveExitAttack);
            }
        }
    }

    Finish();
}

