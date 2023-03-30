// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = mesh;

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	projectileMovementComponent->MaxSpeed = speed;
	projectileMovementComponent->InitialSpeed = speed;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	SetLifeSpan(lifeSpan);	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const float AProjectile::GetProjectileSpeed() const
{
	return speed;
}

// void AProjectile::SetPredictionSpeed(FVector targetLocation)
// {
// 	if (!projectileMovementComponent)
//     {
//         return;
//     }


//     FVector startLocation = GetActorLocation();
//     FVector LaunchVelocity;

//     float distance = (targetLocation - startLocation).Size();
//     float timeToTarget = distance / speed;
//     FVector velocityToTarget = timeToTarget * projectileMovementComponent->Velocity;
//     FVector endLocation = startLocation + velocityToTarget;
//     UE_LOG(LogTemp, Display, TEXT("START message %s"), *startLocation.ToString());
//     UE_LOG(LogTemp, Display, TEXT("END message %s"), *endLocation.ToString());
//     // The gravity scale of the world
//     // float GravityScale = GetWorld()->GetGravityZ() / -980.0f;

//     // Calculate the launch velocity
//     bool bHasValidSolution = UGameplayStatics::SuggestProjectileVelocity(
//         this,
//         LaunchVelocity,
//         startLocation,
//         endLocation,
//         speed,
//         false,
//         0.0f,
//         0.0f,
//         ESuggestProjVelocityTraceOption::TraceFullPath,
//         FCollisionResponseParams::DefaultResponseParam,
//         TArray<AActor*>(),
//         true
//     );

//     if (bHasValidSolution)
//     {
//         // Set the velocity and activate the projectile movement component
//         // projectileMovementComponent->SetVelocityInLocalSpace(LaunchVelocity);
// 		// projectileMovementComponent->Velocity = LaunchVelocity;
// 		// projectileMovementComponent->bRotationFollowsVelocity = true;
//         // projectileMovementComponent->Activate();
//         projectileMovementComponent->Velocity = LaunchVelocity;
//     }
// }

void AProjectile::SetPredictionSpeed(FVector targetLocation)
{
	if (!projectileMovementComponent)
    {
        return;
    }

    FVector startLocation = GetActorLocation();
    FVector LaunchVelocity;
    
    float distance = FVector::Distance(startLocation, targetLocation);
    float timeToTarget = distance / speed;
    FVector velocityToTarget = projectileMovementComponent->Velocity * timeToTarget;
    FVector endLocation = startLocation + velocityToTarget;
    UE_LOG(LogTemp, Display, TEXT("START message %s"), *startLocation.ToString());
    UE_LOG(LogTemp, Display, TEXT("END message %s"), *endLocation.ToString());
    // The gravity scale of the world
    // float GravityScale = GetWorld()->GetGravityZ() / -980.0f;

    // Calculate the launch velocity
    bool bHasValidSolution = UGameplayStatics::SuggestProjectileVelocity(
        this,
        LaunchVelocity,
        startLocation,
        endLocation,
        speed,
        false,
        0.0f,
        0.0f,
        ESuggestProjVelocityTraceOption::TraceFullPath,
        FCollisionResponseParams::DefaultResponseParam,
        TArray<AActor*>(),
        true
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

