// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackBehavior_Ixion_BAShockwave.generated.h"

UCLASS()
class BULLETHELL_API AAttackBehavior_Ixion_BAShockwave : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttackBehavior_Ixion_BAShockwave();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetSpeed(float newSpeed);
	void SetDamage(float newDamage);
	void SetHitCooldown(float newHitCooldown);
	void SetScaleGrow(bool newScaleGrow);
	void SetZGrow(bool newZGrow);
	void SetZGrowSpeed(float newZGrowSpeed);
	void SetMove(bool newMove);
	void SetMovementSpeed(float newMovementSpeed);

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	float damage = 10.f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	float speed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	float hitCooldown = .3f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	bool scaleGrow = false;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	bool zGrow = false;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	float zGrowSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	bool move = false;

	UPROPERTY(EditAnywhere, Category="Basic Attack: Shockwave")
	float movementSpeed = 8.0f;

	APawn* playerPawn = nullptr;

	bool alreadyHit = false;

	FTimerHandle cooldownTimerHandle;

	void GrowScale(float DeltaTime);
	void Move(float DeltaTime);

	// UFUNCTION()
	// void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
