// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"


void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();
    playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    
    if (AIBehavior && playerPawn) {
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

const USceneComponent* AShooterAIController::GetProjectileSpawnPoint() const
{
    AShooterCharacter* character = Cast<AShooterCharacter>(GetPawn());

    if (!character) {
        return nullptr;
    }

    return character->GetProjectileSpawnPoint();
}

const FRotator AShooterAIController::LookAt(const FVector target) const
{
	return UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), target);
}

void AShooterAIController::LookAtPlayer()
{
    if (!playerPawn) {
        return;
    }

    GetPawn()->SetActorRotation(LookAt(playerPawn->GetActorLocation()));
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // if (!playerPawn) return;

    // if (LineOfSightTo(playerPawn)) {
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), playerPawn->GetActorLocation());
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), playerPawn->GetActorLocation());
    // }
    // else {
    //     GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    // }
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* controlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if (controlledCharacter) {
        return controlledCharacter->IsDead();
    }

    return false;
}

const float AShooterAIController::GetFlyOffset()
{
    return flyOffset;
}
