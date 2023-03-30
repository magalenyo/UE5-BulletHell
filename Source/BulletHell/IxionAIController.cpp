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
    UE_LOG(LogTemp, Display, TEXT("Your messaasdge %s"), *GetPawn()->GetActorForwardVector().ToString());
    DrawDebugLine(GetWorld(), positionToLookAt, positionToLookAt + FVector(0, 0, 500), FColor::Red, false, -1.0f, 0, 1.0f);

    return positionToLookAt;
}


void AIxionAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (isAttacking) {
        LookAtPlayer();
        // BasicAttack();
    }
}

void AIxionAIController::StartBasicAttack()
{
    isAttacking = true;

    GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BasicAttack, .05, true);
}

void AIxionAIController::BasicAttack()
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
	FRotator rotation = projectileSpawnPoint->GetComponentRotation();

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
        projectile->SetPredictionSpeed(randomTargetPosition);
        UGameplayStatics::FinishSpawningActor(projectile, transform);

        projectile->SetOwner(this);
        // https://forums.unrealengine.com/t/spawn-actor-with-expose-on-spawn-properties-in-c/330104 -> deferred
        // https://forums.unrealengine.com/t/spawning-an-actor-with-parameters/329151/3 -> deferred
        // https://www.youtube.com/watch?v=mHofF0ddts0 -> Suggest projectile speed
    }

    GetWorldTimerManager().ClearTimer(fireRateTimerHandle);

    currentBulletsBasicAttack++;
    if (currentBulletsBasicAttack == bulletsBasicAttack){
        isAttacking = false;
        onBasicAttackFinishedDelegate.ExecuteIfBound();
        currentBulletsBasicAttack = 0;
    }
    else{
        float nextTime = FMath::RandRange(.05f, .15f);
        GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &AIxionAIController::BasicAttack, nextTime, true);
    }
}
