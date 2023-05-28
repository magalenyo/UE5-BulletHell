// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack.h"
#include "ShooterCharacter.h"


UAttack::UAttack()
{
}

UAttack::UAttack(AShooterAIController* newOwner)
{
    owner = newOwner;
}

UAttack::~UAttack()
{
}

void UAttack::SetOwner(AShooterAIController* newOwner)
{
    owner = newOwner;
}

AShooterAIController* UAttack::GetOwner() const
{
    return owner;
}

APawn* UAttack::GetPawn() const
{
    if (!owner) {
        return nullptr;
    }

    return owner->GetPawn();
}

const USceneComponent* UAttack::GetDefaultSpawnPoint() const
{
    AShooterCharacter* character = Cast<AShooterCharacter>(GetPawn());

    if (!character) {
        return nullptr;
    }

    return character->GetProjectileSpawnPoint();
}
