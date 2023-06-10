// Fill out your copyright notice in the Description page of Project Settings.


#include "AttacksBehaviors/AttackBehavior_Ixion_HALaserBeam.h"

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
	
	initialRotation = GetActorRotation();
	initialRotation.Pitch = initialDegree;

	SetLifeSpan(duration);
}

void AAttackBehavior_Ixion_HALaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FRotator lerpedRotation = FMath::Lerp(initialDegree, TargetRotation, InterpolationSpeed);

	//FMath::Lerp(FRotator(0, 0, 0), FRotator())

	//SetActorRotation();

	//SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, RotateValue, 0.0f)), 0.01f));
		//float CurrentYawRate = FMath::Lerp(CurrentYawRate, RequestedYawRate, DeltaTime * Acceleration);
	currentTime += DeltaTime;
	//UE_LOG(LogTemp, Display, TEXT("Alpha %f"), currentTime / duration);
	UE_LOG(LogTemp, Display, TEXT("Alpha %f .Smooth %f"), currentTime / duration, FMath::SmoothStep(initialDegree, finalDegree, currentTime / duration));


	//FRotator rotation = GetActorRotation() + FRotator(FMath::SmoothStep(initialDegree, finalDegree, currentTime / duration), 0, 0);
	//FRotator rotation = GetActorRotation() + FRotator(-180 + FMath::SmoothStep(0.0f, 290.0f, currentTime / duration), 0, 0);
	//FRotator rotation = GetActorRotation() + FRotator(FMath::Lerp(initialDegree, finalDegree, FMath::SmoothStep(0.0f, 1.0f, currentTime / duration)), 0, 0);

	//FRotator rotation = GetActorRotation() + FRotator(FMath::InterpEaseInOut(initialDegree, finalDegree, currentTime / duration, 4), 0, 0);
	//FRotator rotation = GetActorRotation() + FRotator(FMath::Lerp(initialDegree, finalDegree, currentTime / duration), 0, 0);
	//rotation.Pitch += FMath::Lerp(initialDegree, finalDegree, currentTime / duration);

	//FQuat quatRotation = FQuat(FRotator(FMath::Lerp(initialDegree, finalDegree, currentTime / duration), 0 ,0));
	FQuat quatRotation = FQuat(FRotator(FMath::InterpEaseInOut(initialDegree, finalDegree, currentTime / duration, 8), 0, 0));
	//UE_LOG(LogTemp, Display, TEXT("Rotation: %s"), *rotation.ToString());


	//SetActorRotation(rotation);
	//AddActorLocalRotation(quatRotation);
	SetActorRotation(initialRotation.Quaternion() + quatRotation);
}

void AAttackBehavior_Ixion_HALaserBeam::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
