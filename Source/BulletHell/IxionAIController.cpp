// Fill out your copyright notice in the Description page of Project Settings.


#include "IxionAIController.h"
#include "Projectile/Projectile.h"
#include "ShooterCharacter.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "BT/Ixion/BTTask_Ixion_BasicAttack.h"


void AIxionAIController::BeginPlay()
{
    Super::BeginPlay();
}

const FVector AIxionAIController::GetPredictedDestination() const
{
    AShooterCharacter* character = Cast<AShooterCharacter>(playerPawn);

    if (!character) {
        return FVector(0, 0, 0);
    }

    FVector enemyPosition = playerPawn->GetActorLocation();
    // float projectileSpeed = projectile.GetProjectileSpeed();
    float projectileSpeed = 3000;
    float timeToReach = (enemyPosition - GetPawn()->GetActorLocation()).Size() / projectileSpeed;
    FVector positionToLookAt = enemyPosition + (GetPawn()->GetActorForwardVector() * character->GetMovementSpeed() * timeToReach);
    DrawDebugLine(GetWorld(), positionToLookAt, positionToLookAt + FVector(0, 0, 500), FColor::Red, false, -1.0f, 0, 1.0f);

    return positionToLookAt;
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
    FVector randomTargetPosition =  (usePredicted ? GetPredictedDestination() : playerPawn->GetActorLocation()) + FVector(randomX, randomY, -heightOffset + randomZ);
    // FRotator randomRotation = LookAt(location, playerPawn->GetActorLocation() + FVector(randomX, randomY, -heightOffset));
    // FVector initialDestination = usePredicted ? GetPredictedDestination() : playerPawn->GetActorLocation();
    FRotator randomRotation = LookAt(location, randomTargetPosition);

	// AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, location, randomRotation);
	// projectile->SetOwner(this);
    FTransform transform = FTransform(randomRotation, location);
    AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(projectileClass, transform);
    if (projectile)
    {
        UGameplayStatics::FinishSpawningActor(projectile, transform);
        
        projectile->SetPredictionSpeed(randomTargetPosition, playerPawn->GetVelocity());

        projectile->SetOwner(this);
        // https://forums.unrealengine.com/t/spawn-actor-with-expose-on-spawn-properties-in-c/330104 -> deferred
        // https://forums.unrealengine.com/t/spawning-an-actor-with-parameters/329151/3 -> deferred
        // https://www.youtube.com/watch?v=mHofF0ddts0 -> Suggest projectile speed
    }

    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);

    currentBulletsBasicAttack++;
    if (currentBulletsBasicAttack == bulletsBasicAttack){
        FinishAttack();
    }
    else{
        float nextTime = FMath::RandRange(.05f, .15f);
        GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BAMachineGun, nextTime, true);
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
    currentBulletsBasicAttack = 0;
    currentWaveVortex = 0;
    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);
    if (isBasicAttack) {
        onBasicAttackFinishedDelegate.ExecuteIfBound();
    } else {
        onHeavyAttackFinishedDelegate.ExecuteIfBound();
    }
}
