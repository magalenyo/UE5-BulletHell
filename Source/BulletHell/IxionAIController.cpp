// Fill out your copyright notice in the Description page of Project Settings.


#include "IxionAIController.h"
#include "Projectile/Projectile.h"
#include "ShooterCharacter.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BT/Ixion/BTTask_Ixion_BasicAttack.h"


void AIxionAIController::BeginPlay()
{
    Super::BeginPlay();
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
            GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BAMachineGun, .05, true);
        break;
        case EIxionBasicAttack::BURST:
            GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BABurst, .05, true);
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

void AIxionAIController::BAMachineGun()
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

    float randomX = FMath::RandRange(-radiusOffset, radiusOffset);
    float randomY = FMath::RandRange(-radiusOffset, radiusOffset);
    float randomZ = FMath::RandRange(-radiusOffset, radiusOffset);
    FVector randomTargetPosition = playerPawn->GetActorLocation() + FVector(randomX, randomY, -heightOffset + randomZ);
    FRotator randomRotation = LookAt(location, randomTargetPosition);

    FTransform transform = FTransform(randomRotation, location);
    AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(projectileClass, transform);
    if (projectile)
    {
        UGameplayStatics::FinishSpawningActor(projectile, transform);
        
        projectile->SetPredictionSpeed(randomTargetPosition, playerPawn->GetVelocity());

        projectile->SetOwner(this);
    }

    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);

    currentBulletsMachineGun++;
    if (currentBulletsMachineGun == bulletsBasicAttack){
        FinishAttack();
    }
    else{
        float nextTime = FMath::RandRange(nextAttackMinTimeMachineGun, nextAttackMaxTimeMachineGun);
        GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BAMachineGun, nextTime, true);
    }
}

void AIxionAIController::BABurst()
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

    FVector directionTowardsPlayer = (playerPawn->GetActorLocation() - location).GetSafeNormal();

    for (int i = 0; i < bulletsPerWaveBurstAttack; ++i) {
        FVector direction = UKismetMathLibrary::RandomUnitVectorInConeInRadians(directionTowardsPlayer, UKismetMathLibrary::DegreesToRadians(angleBurstAttack));
        FTransform transform = FTransform(direction.Rotation(), location);
        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(projectileClass, transform);
        if (projectile)
        {
            UGameplayStatics::FinishSpawningActor(projectile, transform);
            projectile->SetSpeed(speedBurstAttack);
            projectile->SetOwner(this);
        }
    }

    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);

	currentWaveBurst++;

    if (currentWaveBurst == wavesBurstAttack) {
        FinishAttack();
    }
    else {
        float nextTime = durationBurstAttack / wavesBurstAttack;
        GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BABurst, nextTime, true);
    }
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
    FVector targetLocation = playerPawn->GetActorLocation();
    FRotator rotationTowardsPlayer = LookAt(targetLocation, location);

    float angleToRotate = 360.0f / startPointsPerWaveVortex;
    float angleOffsetPerWave = rotationSpeedVortex / durationVortex;
    float angleExtraPerWave = 360 / wavesPerPointVortex;

    for (int i = 0; i < startPointsPerWaveVortex; ++i) {
        float extra = angleExtraPerWave * currentWaveVortex * angleOffsetPerWave;
        FRotator pointRotation = rotationTowardsPlayer + FRotator((angleToRotate * i) + extra, 90, 0);

        // if (i == 0) {
        //     UE_LOG(LogTemp, Display, TEXT("TowardsPlayer: %s Initial rotation: %f, Extra: %f FInal: %s"), *rotationTowardsPlayer.ToString(), angleToRotate, extra, *pointRotation.ToString());
        // }

        FVector bulletLocation = location;
        FTransform transform = FTransform(pointRotation, bulletLocation);

        AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(projectileClass, transform);
        if (projectile)
        {
            projectile->SetSpeed(speedExitAttack);
            UGameplayStatics::FinishSpawningActor(projectile, transform);

            projectile->SetOwner(this);
        }
    }

    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);

    currentWaveVortex++;
    if (currentWaveVortex == wavesPerPointVortex) {
        FinishAttack(false);
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
    currentBulletsMachineGun = 0;
    currentWaveVortex = 0;
    currentWaveBurst = 0;
    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);
    if (isBasicAttack) {
        onBasicAttackFinishedDelegate.ExecuteIfBound();
    } else {
        onHeavyAttackFinishedDelegate.ExecuteIfBound();
    }
}
