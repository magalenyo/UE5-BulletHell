// Fill out your copyright notice in the Description page of Project Settings.


#include "Attacks/Attack_Ixion_HADescendRush.h"
#include "IxionAIController.h"
#include "Components/SplineComponent.h"
#include "ComponentFollowSpline.h"

void UAttack_Ixion_HADescendRush::Start()
{
    if (!splineClass) {
        Finish();
        return;
    }
    
    spline = GetWorld()->SpawnActor<AActor>(splineClass, FVector::ZeroVector, FRotator(0, FMath::RandRange(0.0f, 360.0f), 0));   // Should randomize rotation
    USplineComponent* splineComponent = Cast<USplineComponent>(spline->GetComponentByClass(USplineComponent::StaticClass()));

    if (!splineComponent) {
        Finish();
        return;
    }

    followSplineComponent = Cast<UComponentFollowSpline>(GetOwner()->GetPawn()->GetComponentByClass(UComponentFollowSpline::StaticClass()));
    if (!followSplineComponent) {
        Finish();
        return;
    }

    followSplineComponent->SetSpline(splineComponent, accelerationCurve, duration, drawDebug);
    followSplineComponent->StartSpline();
    followSplineComponent->onSplineFinishedDelegate.BindUFunction(this, FName("Finish"));
}

void UAttack_Ixion_HADescendRush::Finish()
{
    followSplineComponent->onSplineFinishedDelegate.Unbind();

    UE_LOG(LogTemp, Display, TEXT("DESTROYING BP"));
    if (spline) {
        spline->Destroy();
    }

    AIxionAIController* ixion = Cast<AIxionAIController>(GetOwner());
    if (ixion) {
        ixion->FinishAttack();
    }
    ixion->FinishAttack(false);
}
