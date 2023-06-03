// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_BASweep.h"
#include "Projectile/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "IxionAIController.h"

void UAttack_Ixion_BASweep::Start()
{
	currentWave = 1;
	nextTime = durationPerWave / bulletsPerWave;
	horizontalAnglePerBullet = horizontalAngle / bulletsPerWave;
	verticalAnglePerBullet = verticalAngle / lines;
	const USceneComponent* spawnPointScene = GetDefaultSpawnPoint();
	if (!spawnPointScene) {
		Finish();
	}
	spawnPoint = spawnPointScene->GetComponentLocation();
	GetWorld()->GetTimerManager().SetTimer(fireRateTimerHandle, this, &UAttack_Ixion_BASweep::BASweep, nextTime, true);
}

void UAttack_Ixion_BASweep::Finish()
{
	GetWorld()->GetTimerManager().ClearTimer(fireRateTimerHandle);
	AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
	if (ixion) {
		ixion->FinishAttack();
	}
}

void UAttack_Ixion_BASweep::BASweep()
{
	APawn* playerPawn = GetOwner()->GetPlayerPawn();

	if (!playerPawn) {
		return;
	}

	FRotator currentRotation = GetPawn()->GetActorRotation();
	FVector playerPosition = playerPawn->GetActorLocation();

	// Straight lines
	float horizontalRotationAngle = direction > 0 ? -(horizontalAngle / 2) + horizontalAnglePerBullet * currentProjectile
										           : (horizontalAngle / 2) - horizontalAnglePerBullet * currentProjectile;

	for (int i = 0; i < lines; ++i) {
		float verticalRotationAngle = -(verticalAngle / lines) + i * verticalAnglePerBullet;
		FTransform transform = FTransform(currentRotation + FRotator(verticalRotationAngle, horizontalRotationAngle, 0) + angleOffset, spawnPoint);
		AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
		if (projectile)	{
			UGameplayStatics::FinishSpawningActor(projectile, transform);
			projectile->SetSpeed(bulletSpeed);
			projectile->SetOwner(GetOwner());
		}
	}

	// Extra line
	float verticalRotationHalfway = verticalAnglePerBullet / 2;
	FTransform transform = FTransform(currentRotation + FRotator(verticalRotationHalfway, horizontalRotationAngle, 0) + angleOffset, spawnPoint);
	AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
	if (projectile) {
		UGameplayStatics::FinishSpawningActor(projectile, transform);
		projectile->SetSpeed(bulletSpeed);
		projectile->SetOwner(GetOwner());
	}

	// Random bullets
	int extraBullets = FMath::RandRange(0, 3);
	for (int i = 0; i < extraBullets; ++i) {
		float randomX = FMath::RandRange(-extraBulletOffsetDegree, extraBulletOffsetDegree);
		float randomY = FMath::RandRange(-extraBulletOffsetDegree, extraBulletOffsetDegree);
		float randomZ = FMath::RandRange(-extraBulletOffsetDegree, extraBulletOffsetDegree);
		FTransform transformRandom = FTransform(currentRotation + FRotator(verticalRotationHalfway, horizontalRotationAngle, 0) + FRotator(randomX, randomY, randomZ) + angleOffset, spawnPoint);

		FTimerHandle localTimerHandle;
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		FTimerDelegate timerDelegate;
		timerDelegate.BindLambda([&, transformRandom]()
			{
				AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transformRandom);
				if (projectile) {
					UGameplayStatics::FinishSpawningActor(projectile, transformRandom);
					projectile->SetSpeed(bulletSpeed);
					projectile->SetOwner(GetOwner());
				}

				timerManager.ClearTimer(localTimerHandle);
			});

		float nextTimeExtraBullet = FMath::RandRange(0.007f, extraBulletTime);
		timerManager.SetTimer(localTimerHandle, timerDelegate, nextTimeExtraBullet, false);
	}


	if (currentProjectile == bulletsPerWave) {
		if (currentWave == waves) {
			Finish();
		}
		else {
			currentWave++;
			currentProjectile = 1;
			direction *= -1;
		}
	}
	else {
		currentProjectile++;
	}
}
