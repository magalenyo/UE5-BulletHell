// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_HADescendRush.h"
#include "IxionAIController.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "ComponentFollowSpline.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/Projectile.h"
#include "AttacksBehaviors/AttackBehavior_Ixion_BAShockwave.h"
#include "Sound/SoundCue.h"
#include "IxionCharacter.h"

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
    //followSplineComponent->onSplineFinishedDelegate.BindUFunction(this, FName("Finish"));
    followSplineComponent->AddEvent(attackPoint * duration, onPerformAttack);
    projectilesVortex.resize(startPointsPerWaveVortex * wavesPerPointVortex);

    PlayDescendAudio();
    StartTrail();
}

void UAttack_Ixion_HADescendRush::Finish()
{
    followSplineComponent->onSplineFinishedDelegate.Unbind();
    projectilesVortex.clear();
    currentWaveVortex = 0;
    currentWaveVortexReposition = 0;

    if (spline) {
        spline->Destroy();
    }

    StopTrail();
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
    
    PlayCameraShake();
    FireVortex();
    FireShockwaves();
}

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

            projectile->SetSpeed(bulletSpeedVortex);
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetVelocity(finalVelocity * bulletSpeedVortex);
            projectile->SetOwner(GetOwner());
            projectile->SetLifeSpan(bulletDuration);
        }
        projectilesVortex[i + (currentWaveVortex * startPointsPerWaveVortex)] = projectile;
    }

    GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);

    float timePerWave = durationVortex / wavesPerPointVortex;
    float timeToReposition = timePerWave * currentWaveVortex;

    if (currentWaveVortex == 0) {
        //GetWorld()->GetTimerManager().SetTimer(retargetWaveVortexTimerHandle, this, &UAttack_Ixion_HADescendRush::RepositionVortexProjectiles, 0.05, false);º
        RepositionVortexProjectiles();
    }

    currentWaveVortex++;
    if (currentWaveVortex != wavesPerPointVortex) {
        float nextTime = durationVortex / wavesPerPointVortex;
        GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_HADescendRush::FireVortex, nextTime, true);
    }
}

void UAttack_Ixion_HADescendRush::FireShockwaves()
{
    if (shockwaveClass) {
        USceneComponent* targetMesh = Cast<USceneComponent>(spline->GetDefaultSubobjectByName("ShockwaveSpawnPoint"));

        if (!targetMesh) {
            Finish();
        }

        FVector spawnPoint = targetMesh->GetComponentLocation();
        FRotator rotation = GetOwner()->LookAt(targetLocation, fireLocation);     // TODO: Should get Actor's Rotation but that won't work for now

        AAttackBehavior_Ixion_BAShockwave* shockwave1 = GetWorld()->SpawnActor<AAttackBehavior_Ixion_BAShockwave>(shockwaveClass);
        if (shockwave1) {
            shockwave1->SetLifeSpan(shockwaveDuration);
            shockwave1->SetMove(true);
            shockwave1->SetMovementSpeed(shockwaveMovementSpeed);
            shockwave1->SetActorLocation(spawnPoint);
            shockwave1->SetActorRotation(rotation);
        }

        AAttackBehavior_Ixion_BAShockwave* shockwave2 = GetWorld()->SpawnActor<AAttackBehavior_Ixion_BAShockwave>(shockwaveClass);
        if (shockwave2) {
            shockwave2->SetLifeSpan(shockwaveDuration);
            shockwave2->SetMove(true);
            shockwave2->SetMovementSpeed(shockwaveMovementSpeed);
            shockwave2->SetActorLocation(spawnPoint);
            shockwave2->SetActorRotation(FRotator(rotation.Pitch, rotation.Yaw + 180, rotation.Roll));
        }
    }
}

void UAttack_Ixion_HADescendRush::RepositionVortexProjectiles() 
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
            FVector direction = FVector::CrossProduct(projectile->GetVelocity().GetSafeNormal(), (targetLocation - projectile->GetActorLocation()).GetSafeNormal());            //TODO: Should follow a parallel from origin to target, but not currently working, so a far destination will do for now
            FVector directionToPlayer = (targetLocation - projectile->GetActorLocation()).GetSafeNormal();
            projectile->SetVelocity(directionToPlayer * projectile->GetProjectileSpeed());
        }
    }
    float timePerWave = durationExpanseVortex / wavesPerPointVortex;
    float timeToReposition = timePerWave * 2;
    currentWaveVortexReposition++;

    GetWorld()->GetTimerManager().ClearTimer(retargetWaveVortexTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(retargetWaveVortexTimerHandle, this, &UAttack_Ixion_HADescendRush::RepositionVortexProjectiles, timeToReposition, false);
}

void UAttack_Ixion_HADescendRush::PlayCameraShake()
{
    if (!cameraShake) {
        return;
    }

    APawn* playerPawn = GetOwner()->GetPlayerPawn();
    if (!playerPawn) {
        return;
    }

    UGameplayStatics::PlayWorldCameraShake(
        GetWorld(),
        cameraShake,
        playerPawn->GetActorLocation(),
        0,
        2000
    );
}

void UAttack_Ixion_HADescendRush::PlayDescendAudio()
{
    if (!descendCue) {
        return;
    }

    UGameplayStatics::SpawnSoundAttached(descendCue, GetOwner()->GetPawn()->GetRootComponent());
}

void UAttack_Ixion_HADescendRush::StartTrail() const
{
    AIxionCharacter* pawn = Cast<AIxionCharacter>(GetPawn());
    if (!pawn) {
        return;
    }

    pawn->StartTrail();
}

void UAttack_Ixion_HADescendRush::StopTrail() const
{
    AIxionCharacter* pawn = Cast<AIxionCharacter>(GetPawn());
    if (!pawn) {
        return;
    }

    pawn->StopTrail();
}