
#include "AttacksBehaviors/AttackBehavior_Ixion_BAShockwave.h"
#include "Kismet/GameplayStatics.h"

AAttackBehavior_Ixion_BAShockwave::AAttackBehavior_Ixion_BAShockwave()
{
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(root);
}

void AAttackBehavior_Ixion_BAShockwave::BeginPlay()
{
	Super::BeginPlay();

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	mesh->OnComponentHit.AddDynamic(this, &AAttackBehavior_Ixion_BAShockwave::OnHit);

	// SetLifeSpan(5.0f);
}

void AAttackBehavior_Ixion_BAShockwave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector scale = mesh->GetComponentScale();
	mesh->SetWorldScale3D(FVector(scale.X + speed * DeltaTime, scale.Y + speed * DeltaTime, scale.Z));
}

void AAttackBehavior_Ixion_BAShockwave::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

void AAttackBehavior_Ixion_BAShockwave::SetDamage(float newDamage)
{
	damage = newDamage;
}

void AAttackBehavior_Ixion_BAShockwave::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("Your message %s"), playerPawn);
	if (OtherActor != playerPawn) {
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("HIT"));
	
	// FPointDamageEvent damageEvent(
	// 	damage,
	// 	hit,
	// 	shotDirection,
	// 	nullptr
	// );
	// AController* ownerController = GetOwnerController();
	// hitActor->TakeDamage(damage, damageEvent, ownerController, this);
}

