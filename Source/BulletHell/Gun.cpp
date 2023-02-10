// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(
		muzzleFlash,
		mesh,
		TEXT("MuzzleFlashSocket")
	);

	UGameplayStatics::SpawnSoundAttached(
		muzzleSound,
		mesh,
		TEXT("MuzzleFlashSocket")
	);

	FHitResult hit;
	FVector shotDirection;
	bool success = GunTrace(hit, shotDirection);
	if (success) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			impactEffect,
			hit.Location,
			shotDirection.Rotation()
		);

		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			impactSound,
			hit.Location
		);

		AActor* hitActor = hit.GetActor();
		if (hitActor) {
			FPointDamageEvent damageEvent(
				damage,
				hit,
				shotDirection,
				nullptr
			);
			AController* ownerController = GetOwnerController();
			hitActor->TakeDamage(damage, damageEvent, ownerController, this);
		}
	}
	
}

bool AGun::GunTrace(FHitResult &hit, FVector& shotDirection)
{
	AController* ownerController = GetOwnerController();
	if (!ownerController) return false;

	FVector location;
	FRotator rotation;
	ownerController->GetPlayerViewPoint(location, rotation);
	shotDirection = -rotation.Vector();		// where it's being shot from
	FVector end = location + rotation.Vector() * maxRange;

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(
		hit,
		location,
		end,
		ECollisionChannel::ECC_GameTraceChannel1,
		params
	);
}

AController* AGun::GetOwnerController() const
{
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (!ownerPawn) return nullptr;
	return ownerPawn->GetController();
}

