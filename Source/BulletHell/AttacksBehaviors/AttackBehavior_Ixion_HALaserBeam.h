// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackBehavior_Ixion_HALaserBeam.generated.h"

UCLASS()
class BULLETHELL_API AAttackBehavior_Ixion_HALaserBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttackBehavior_Ixion_HALaserBeam();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Laser Beam")
	float duration = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Laser Beam")
	float damage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Laser Beam")
	float hitCooldown = .3f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Shockwave")
	float initialDegree = -180.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Shockwave")
	float finalDegree = 110.0f;

	float currentTime = 0.0f;
	APawn* playerPawn = nullptr;

	FRotator initialRotation = FRotator::ZeroRotator;
	bool alreadyHit = false;
	FTimerHandle cooldownTimerHandle;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
