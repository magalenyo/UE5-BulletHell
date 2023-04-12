// Fill out your copyright notice in the Description page of Project Settings.


#include "IxionAIController.h"
#include "Projectile/Projectile.h"
#include "ShooterCharacter.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "BT/Ixion/BTTask_Ixion_BasicAttack.h"

AIxionAIController::AIxionAIController() 
{
    // attacktest = NewObject<UAttack_Ixion_BAMachineGun>(this, UAttack_Ixion_BAMachineGun::StaticClass(), FName("attacktest"));
    // attacktest = NewObject<UAttack_Ixion_BAMachineGun>(this);
    // baMachineGun = CreateDefaultSubobject<UAttack_Ixion_BAMachineGun>(TEXT("BA Machine Gun"),);
}

void AIxionAIController::BeginPlay()
{
    Super::BeginPlay();

    if (baMachineGun) {
        baMachineGun->SetOwner(this);
    }

    if (baBurst) {
        baBurst->SetOwner(this);
    }
}

void AIxionAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (isAttacking && mustLookAtPlayer) {
        LookAtPlayer();
    }
}

void AIxionAIController::StartBasicAttack(const EIxionBasicAttack attack)
{
    isAttacking = true;

    switch(attack) {
        case EIxionBasicAttack::EXIT_ATTACK:
            GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BAExit, .05, true);
        break;
        case EIxionBasicAttack::MACHINE_GUN:
            mustLookAtPlayer = true;
            if (baMachineGun) {
                baMachineGun->Start();
            }
            else{
                FinishAttack();
            }
        break;
        case EIxionBasicAttack::BURST:
            mustLookAtPlayer = true;
            if (baBurst) {
                baBurst->Start();
            }
            else{
                FinishAttack();
            }
        break;
        default:
            AttackDefault();
        break;
    }
}

void AIxionAIController::StartHeavyAttack(const EIxionHeavyAttack attack)
{
    isAttacking = true;

    switch(attack) {
        case EIxionHeavyAttack::VORTEX:
            mustLookAtPlayer = true;
            targetPositionVortex = playerPawn->GetActorLocation();
            projectilesVortex.resize(startPointsPerWaveVortex * wavesPerPointVortex);
            LookAtPlayer();
            GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::HAVortex, .05, true);
        break;
        default:
            AttackDefault();
        break;
    }
}

void AIxionAIController::AttackDefault()
{
    UE_LOG(LogTemp, Display, TEXT("AIxionAIController::StartAttack: Invalid attack"));
    isAttacking = false;
    onBasicAttackFinishedDelegate.ExecuteIfBound();
}

void AIxionAIController::BAExit()
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
        FTransform transform = FTransform(LookAt(randomPosition), location);
        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(projectileClass, transform);
        if (projectile)
        {
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetOwner(this);
            projectile->SetLifeSpan(lifeSpanExitAttack);
            projectile->SetSpeed(speedExitAttack);
            if (useDecelerationCurveExitAttack && decelerationCurveExitAttack) {
                projectile->SetDecelerationCurve(decelerationCurveExitAttack);
            }
        }
    }

    FinishAttack();
    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);
}




void AIxionAIController::HAVortex() 
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
    FRotator rotationTowardsPlayer = LookAt(targetPositionVortex, location);

    float angleToRotate = 360.0f / startPointsPerWaveVortex;
    float angleOffsetPerWave = rotationSpeedVortex / durationVortex;
    float angleExtraPerWave = 360 / wavesPerPointVortex;

    for (int i = 0; i < startPointsPerWaveVortex; ++i) {
        float extra = angleExtraPerWave * currentWaveVortex * angleOffsetPerWave;
        FRotator pointRotation = rotationTowardsPlayer + FRotator((angleToRotate * i) + extra, 90, 0);

        FVector bulletLocation = location;
        FTransform transform = FTransform(pointRotation, bulletLocation);

        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(projectileClass, transform);
        if (projectile) {
            FVector velocityOutwards = pointRotation.Vector();
            FVector finalVelocity = FVector::CrossProduct(velocityOutwards, rotationTowardsPlayer.Vector());

            projectile->SetSpeed(speedVortexAttack);
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetVelocity(finalVelocity * speedVortexAttack);
            projectile->SetOwner(this);
        }
        projectilesVortex[i + (currentWaveVortex * startPointsPerWaveVortex)] = projectile;
    }

    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);

    float timePerWave = durationVortex / wavesPerPointVortex;
    float timeToReposition = timePerWave * currentWaveVortex;

    if (currentWaveVortex == 0) {
        GetWorld()->GetTimerManager().SetTimer(retargetWaveVortexTimerHandle, this, &AIxionAIController::RepositionVortexProjectiles, 0.05, false);
    }

    currentWaveVortex++;
    if (currentWaveVortex == wavesPerPointVortex) {
        // FinishAttack(false);
    }
    else{
        float nextTime = durationVortex / wavesPerPointVortex;
        GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::HAVortex, nextTime, true);
    }
}

void AIxionAIController::FinishAttack(bool isBasicAttack) 
{
    isAttacking = false;
    mustLookAtPlayer = false;
    currentWaveVortex = 0;


    currentWaveVortexReposition = 0;
    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);
    GetWorldTimerManager().ClearTimer(retargetWaveVortexTimerHandle);
    if (isBasicAttack) {
        onBasicAttackFinishedDelegate.ExecuteIfBound();
    } else {
        onHeavyAttackFinishedDelegate.ExecuteIfBound();
    }
}

void AIxionAIController::RepositionVortexProjectiles() 
{
    if (currentWaveVortexReposition == wavesPerPointVortex) {
        GetWorldTimerManager().ClearTimer(retargetWaveVortexTimerHandle);
        FinishAttack(false);
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

    GetWorldTimerManager().ClearTimer(retargetWaveVortexTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(retargetWaveVortexTimerHandle, this, &AIxionAIController::RepositionVortexProjectiles, timeToReposition, false);
}
