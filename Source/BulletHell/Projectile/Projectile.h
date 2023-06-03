// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BULLETHELL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const float GetProjectileSpeed() const;

	void SetPredictionSpeed(FVector targetLocation, FVector targetVelocity);
	void SetSpeed(float newSpeed);
	void SetDecelerationCurve(UCurveFloat* decelerationCurve);
	void SetVelocity(FVector direction);		// Direction should be without applying speed
	void SetGravity(float newGravity);
	void SetDelayedActivation(float delay);


private:

	// PARAMETERS
	UPROPERTY(EditAnywhere, Category = "Combat")
	float damage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float lifeSpan = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float speed = 2500.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float predictionSpeedMitigationFactor = 0.9f;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool debugPredictionTrace = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* hitParticles;
	
	// COMPONENTS
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* projectileMovementComponent;

	// ATTRIBUTES
	float initialDecelerationSpeed = -1.0f;

	bool useDeceleration = false;

	class UTimelineComponent* decelerationTimeline;

	// FUNCTIONS
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnDecelerationTimelineUpdate(float Alpha);

	// TODO: functions to be reviewed (components didn't seem to be disabled)
	void EnableActor();
	void DisableActor();
};
