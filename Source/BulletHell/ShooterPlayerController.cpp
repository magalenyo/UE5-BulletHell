// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"


void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if (HUD) {
        HUD->AddToViewport();
    }
    
}


void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport();
    if (bIsWinner) {
        UUserWidget* winScreen = CreateWidget(this, winScreenClass);
        if (winScreen) {
            winScreen->AddToViewport();
        }
    }
    else{
        UUserWidget* loseScreen = CreateWidget(this, loseScreenClass);
        if (loseScreen) {
            loseScreen->AddToViewport();
        }
    }

    GetWorldTimerManager().SetTimer(
        restartTimer, 
        this,
        &APlayerController::RestartLevel,
        restartDelay
    );
}
