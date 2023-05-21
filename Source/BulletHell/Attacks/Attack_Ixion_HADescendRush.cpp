// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_HADescendRush.h"
#include "IxionAIController.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "ComponentFollowSpline.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/Projectile.h"

void UAttack_Ixion_HADescendRush::Start()
{
    if (!splineClass) {
        Finish();
        return;
    }
    
    spline = GetWorld()->SpawnActor<AActor>(splineClass, FVector::ZeroVector, FRotator(0, FMath::RandRange(0.0f, 360.0f), 0));   // Should randomize rotation
    USplineComponent* splineComponent = Cast<USplineComponent>(spline->GetComponentByClass(USplineComponent::StaticClass()));

    if (!splineComponent) {
        Finish();
        return;
    }

    followSplineComponent = Cast<UComponentFollowSpline>(GetOwner()->GetPawn()->GetComponentByClass(UComponentFollowSpline::StaticClass()));
    if (!followSplineComponent) {
        Finish();
        return;
    }

    FOnTimelineEvent onPerformAttack;
	onPerformAttack.BindUFunction(this, FName("OnPerformAttack"));

    followSplineComponent->SetSpline(splineComponent, accelerationCurve, duration, drawDebug);
    followSplineComponent->StartSpline();
    followSplineComponent->onSplineFinishedDelegate.BindUFunction(this, FName("Finish"));
    followSplineComponent->AddEvent(attackPoint * duration, onPerformAttack);
    projectilesVortex.resize(startPointsPerWaveVortex * wavesPerPointVortex);

}

void UAttack_Ixion_HADescendRush::Finish()
{
    followSplineComponent->onSplineFinishedDelegate.Unbind();

    if (spline) {
        spline->Destroy();
    }

    AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
    ixion->FinishAttack(false);
}

void UAttack_Ixion_HADescendRush::OnPerformAttack()
{
    fireLocation = GetPawn()->GetActorLocation();
    USceneComponent* targetMesh = Cast<USceneComponent>(spline->GetDefaultSubobjectByName("TargetLocation"));

    if (!targetMesh) {
        Finish();
    }
    targetLocation = targetMesh->GetComponentLocation();
    targetLocation.Z = fireLocation.Z;
    
    UE_LOG(LogTemp, Display, TEXT("holaaaa %s"), *targetLocation.ToString());
    FireVortex();
}

// void UAttack_Ixion_HADescendRush::FireVortex()
// {
//     UE_LOG(LogTemp, Display, TEXT("holaaaa"));
//     USceneComponent* targetMesh = Cast<USceneComponent>(spline->GetDefaultSubobjectByName("TargetLocation"));
    
//     if (!targetMesh) {
//         return;
//     }

//     FVector targetLocation = targetMesh->GetComponentLocation();
//     FVector location = GetPawn()->GetActorLocation();

//     FRotator rotationTowardsTarget = GetOwner()->LookAt(location, targetLocation);

//     FTransform transform = FTransform(FRotator(0, rotationTowardsTarget.Yaw, rotationTowardsTarget.Roll), location);
//     AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
//     if (projectile)
//     {
//         UGameplayStatics::FinishSpawningActor(projectile, transform);
        
//         // projectile->SetSpeed(speedBurstAttack);
//         projectile->SetOwner(GetOwner());
//     }

//     // GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);
// }

void UAttack_Ixion_HADescendRush::FireVortex() 
{
    FRotator rotationTowardsPlayer = GetOwner()->LookAt(targetLocation, fireLocation);

    float angleToRotate = 360.0f / startPointsPerWaveVortex;
    float angleOffsetPerWave = rotationSpeedVortex / durationVortex;
    float angleExtraPerWave = 360 / wavesPerPointVortex;

    for (int i = 0; i < startPointsPerWaveVortex; ++i) {
        float extra = angleExtraPerWave * currentWaveVortex * angleOffsetPerWave;
        FRotator pointRotation = rotationTowardsPlayer + FRotator((angleToRotate * i) + extra, 90, 0);

        FTransform transform = FTransform(pointRotation, fireLocation);

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
        //GetWorld()->GetTimerManager().SetTimer(retargetWaveVortexTimerHandle, this, &UAttack_Ixion_HADescendRush::RepositionVortexProjectiles, 0.05, false);
        RepositionVortexProjectiles();
    }

    currentWaveVortex++;
    if (currentWaveVortex != wavesPerPointVortex) {
        float nextTime = durationVortex / wavesPerPointVortex;
        GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_HADescendRush::FireVortex, nextTime, true);
    }
}

void UAttack_Ixion_HADescendRush::RepositionVortexProjectiles() 
{
    if (currentWaveVortexReposition == wavesPerPointVortex) {
        GetWorld()->GetTimerManager().ClearTimer(retargetWaveVortexTimerHandle);
        //Finish();
        return;
    }

    for (int i = (currentWaveVortexReposition * startPointsPerWaveVortex); i < ((currentWaveVortexReposition + 1) * startPointsPerWaveVortex); ++i)
    {
        AProjectile* projectile = projectilesVortex[i];
        if (projectile) {
            FVector direction = FVector::CrossProduct(projectile->GetVelocity().GetSafeNormal(), (targetLocation - projectile->GetActorLocation()).GetSafeNormal());
            FVector directionToPlayer = (targetLocation - projectile->GetActorLocation()).GetSafeNormal();
            projectile->SetVelocity(directionToPlayer * projectile->GetProjectileSpeed());

            //FVector direction = (targetLocation - fireLocation).GetSafeNormal();
            //projectile->SetVelocity((projectile->GetActorLocation().GetSafeNormal() * direction).GetSafeNormal() * projectile->GetProjectileSpeed());
        }
    }
    float timePerWave = durationExpanseVortex / wavesPerPointVortex;
    float timeToReposition = timePerWave * 2;
    currentWaveVortexReposition++;

    GetWorld()->GetTimerManager().ClearTimer(retargetWaveVortexTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(retargetWaveVortexTimerHandle, this, &UAttack_Ixion_HADescendRush::RepositionVortexProjectiles, timeToReposition, false);
}
