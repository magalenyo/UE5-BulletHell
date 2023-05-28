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

	// Random line
	/*AProjectile* projectile = GetWorld()->SpawnActorDeferred<AProjectile>(GetOwner()->GetProjectileClass(), transform);
	if (projectile) {
		UGameplayStatics::FinishSpawningActor(projectile, transform);
		projectile->SetSpeed(bulletSpeed);
		projectile->SetOwner(GetOwner());
	}*/

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
