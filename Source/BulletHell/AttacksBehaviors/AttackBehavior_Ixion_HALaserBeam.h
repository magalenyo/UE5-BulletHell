// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
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
	void SetDuration(float newDuration);
	void SetOwner(AActor* newOwner);

private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Laser Beam")
	float duration = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Laser Beam")
	float easingDegree = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Laser Beam")
	float damage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Laser Beam")
	float hitCooldown = .3f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Laser Beam")
	float initialDegree = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Basic Attack: Laser Beam")
	float finalDegree = 270.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* hitEffect;

	float currentTime = 0.0f;
	APawn* playerPawn = nullptr;
	AActor* owner = nullptr;

	FRotator initialRotation = FRotator::ZeroRotator;
	bool alreadyHit = false;
	FTimerHandle cooldownTimerHandle;

	void Hit();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
