// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = mesh;

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	projectileMovementComponent->MaxSpeed = speed;
	projectileMovementComponent->InitialSpeed = speed;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	SetLifeSpan(lifeSpan);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
}

const float AProjectile::GetProjectileSpeed() const
{
	return speed;
}

void AProjectile::SetPredictionSpeed(FVector targetLocation, FVector targetVelocity)
{
	if (!projectileMovementComponent)
    {
        return;
    }

    FVector startLocation = GetActorLocation();
    FVector LaunchVelocity;
    
    float distance = FVector::Distance(startLocation, targetLocation);
    float timeToTarget = distance / (speed * predictionSpeedMitigationFactor);
    FVector velocityToTarget = targetVelocity * timeToTarget;
    FVector endLocation = targetLocation + velocityToTarget;

    bool bHasValidSolution = UGameplayStatics::SuggestProjectileVelocity(
        this,
        LaunchVelocity,
        startLocation,
        endLocation,
        speed * predictionSpeedMitigationFactor,
        false,
        0.0f,
        10.0f,      // Keep the gravity factor low so it does predict a "linear" trajectory
        ESuggestProjVelocityTraceOption::DoNotTrace,
        FCollisionResponseParams::DefaultResponseParam,
        TArray<AActor*>(),
        debugPredictionTrace
    );

    if (bHasValidSolution)
    {
        // Set the velocity and activate the projectile movement component
        // projectileMovementComponent->SetVelocityInLocalSpace(LaunchVelocity);
		// projectileMovementComponent->Velocity = LaunchVelocity;
		projectileMovementComponent->bRotationFollowsVelocity = true;
        projectileMovementComponent->Velocity = LaunchVelocity;
        projectileMovementComponent->Activate();
    }
    else {
        UE_LOG(LogTemp, Display, TEXT("Your CAN'T"));
    }
}

void AProjectile::SetSpeed(float newSpeed)
{
    speed = newSpeed;
    projectileMovementComponent->MaxSpeed = speed;
	projectileMovementComponent->InitialSpeed = speed;
}

void AProjectile::SetDecelerationCurve(UCurveFloat* decelerationCurve)
{
    useDeceleration = true;
    initialDecelerationSpeed = speed;

    if (!decelerationCurve) {
        return;
    }
    decelerationTimeline = NewObject<UTimelineComponent>(this, UTimelineComponent::StaticClass());

    FOnTimelineFloat progressUpdate;
    progressUpdate.BindUFunction(this, FName("OnDecelerationTimelineUpdate"));
    decelerationTimeline->AddInterpFloat(decelerationCurve, progressUpdate);

    float newLength = lifeSpan;
    float curveLength = decelerationCurve->FloatCurve.GetLastKey().Time;    // Get the length of the curve
    float playRate = curveLength / newLength;                               // Calculate the playback rate to scale the curve
    decelerationTimeline->SetTimelineLength(newLength);
    decelerationTimeline->SetPlayRate(playRate);

    decelerationTimeline->RegisterComponent();
    decelerationTimeline->Play();
    PrimaryActorTick.bCanEverTick = true;
}

void AProjectile::SetVelocity(FVector direction)
{
    projectileMovementComponent->Velocity = direction;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (hitParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			hitParticles,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	Destroy();
}

void AProjectile::OnDecelerationTimelineUpdate(float Alpha)
{
    SetSpeed(initialDecelerationSpeed * Alpha);
}

