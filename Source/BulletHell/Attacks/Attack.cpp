// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack.h"


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

AShooterAIController* UAttack::GetOwner() 
{
    return owner;
}

APawn* UAttack::GetPawn() 
{
    if (!owner) {
        return nullptr;
    }

    return owner->GetPawn();
}
