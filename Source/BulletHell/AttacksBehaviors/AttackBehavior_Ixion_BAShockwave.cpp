
#include "AttacksBehaviors/AttackBehavior_Ixion_BAShockwave.h"
#include "Kismet/GameplayStatics.h"

AAttackBehavior_Ixion_BAShockwave::AAttackBehavior_Ixion_BAShockwave()
{
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mesh);
}

void AAttackBehavior_Ixion_BAShockwave::BeginPlay()
{
	Super::BeginPlay();

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	mesh->OnComponentBeginOverlap.AddDynamic(this, &AAttackBehavior_Ixion_BAShockwave::OnBeginOverlap);

	// SetLifeSpan(5.0f);
}

void AAttackBehavior_Ixion_BAShockwave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector scale = mesh->GetComponentScale();
	mesh->SetWorldScale3D(FVector(scale.X + speed * DeltaTime, scale.Y + speed * DeltaTime, zGrow ? (scale.Z + zGrowSpeed * DeltaTime) : scale.Z));
}

void AAttackBehavior_Ixion_BAShockwave::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

void AAttackBehavior_Ixion_BAShockwave::SetDamage(float newDamage)
{
	damage = newDamage;
}

void AAttackBehavior_Ixion_BAShockwave::SetHitCooldown(float newHitCooldown)
{
	hitCooldown = newHitCooldown;
}

void AAttackBehavior_Ixion_BAShockwave::SetZGrow(bool newZGrow)
{
	zGrow = newZGrow;
}

void AAttackBehavior_Ixion_BAShockwave::SetZGrowSpeed(float newZGrowSpeed)
{
	zGrowSpeed = newZGrowSpeed;
}

void AAttackBehavior_Ixion_BAShockwave::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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

