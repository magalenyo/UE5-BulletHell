// Fill out your copyright notice in the Description page of Project Settings.


#include "AttacksBehaviors/AttackBehavior_Ixion_HALaserBeam.h"
#include "Kismet/GameplayStatics.h"


AAttackBehavior_Ixion_HALaserBeam::AAttackBehavior_Ixion_HALaserBeam()
{
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(root);
}

void AAttackBehavior_Ixion_HALaserBeam::BeginPlay()
{
	Super::BeginPlay();

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	mesh->OnComponentBeginOverlap.AddDynamic(this, &AAttackBehavior_Ixion_HALaserBeam::OnBeginOverlap);

	
	//initialRotation = GetActorRotation();
	//initialRotation.Pitch = initialDegree;

	SetLifeSpan(duration);
}

void AAttackBehavior_Ixion_HALaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;
	FQuat quatRotation = FQuat(FRotator(FMath::InterpEaseInOut(initialDegree, finalDegree, currentTime / duration, easingDegree), 0, 0));
	//FQuat quatRotation = FQuat(FRotator(FMath::Lerp(initialDegree, finalDegree, currentTime / duration), 0, 0));

	mesh->SetRelativeRotation(quatRotation);
	Hit();
}

void AAttackBehavior_Ixion_HALaserBeam::SetDuration(float newDuration)
{
	duration = newDuration;
	SetLifeSpan(duration);
}

void AAttackBehavior_Ixion_HALaserBeam::SetOwner(AActor* newOwner)
{
	owner = newOwner;
}

void AAttackBehavior_Ixion_HALaserBeam::Hit()
{
	FVector end = mesh->GetUpVector() * 10000;

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	params.AddIgnoredActor(owner);

	FHitResult hit;

	bool isHit = GetWorld()->LineTraceSingleByChannel(
		hit,
		GetActorLocation(),
		end,
		ECollisionChannel::ECC_WorldStatic,
		params
	);

	if (!isHit) {
		return;
	}

	if (hitEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitEffect, hit.Location);
	}
}

void AAttackBehavior_Ixion_HALaserBeam::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO Delete
	if (hitEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitEffect, SweepResult.ImpactPoint);
	}

	if (OtherActor != playerPawn) {
		return;
	}

	if (!alreadyHit) {
		alreadyHit = true;
		UE_LOG(LogTemp, Display, TEXT("HIT"));

		GetWorld()->GetTimerManager().SetTimer(cooldownTimerHandle, [this]() {
			alreadyHit = false;
			GetWorld()->GetTimerManager().ClearTimer(cooldownTimerHandle);
			}, hitCooldown, false);
	}
}
