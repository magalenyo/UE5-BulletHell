// Fill out your copyright notice in the Description page of Project Settings.


#include "ComponentFollowSpline.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"

UComponentFollowSpline::UComponentFollowSpline()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UComponentFollowSpline::BeginPlay()
{
	Super::BeginPlay();
}

void UComponentFollowSpline::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UComponentFollowSpline::StartSpline()
{
	if (!spline || !accelerationCurve) {
		return;
	}

	timeline = NewObject<UTimelineComponent>(this, UTimelineComponent::StaticClass());

    FOnTimelineFloat onProgressUpdate;
    onProgressUpdate.BindUFunction(this, FName("OnAccelerationTimelineUpdate"));
    timeline->AddInterpFloat(accelerationCurve, onProgressUpdate);

	FOnTimelineEvent onFinished;
	onFinished.BindUFunction(this, FName("OnAccelerationTimelineFinished"));
	timeline->SetTimelineFinishedFunc(onFinished);

    float newLength = duration;
    float curveLength = accelerationCurve->FloatCurve.GetLastKey().Time;    // Get the length of the curve
    float playRate = curveLength / newLength;                               // Calculate the playback rate to scale the curve
    timeline->SetTimelineLength(newLength);
    timeline->SetPlayRate(playRate);

    timeline->RegisterComponent();
    timeline->Play();
}

void UComponentFollowSpline::SetSpline(USplineComponent* newSpline, UCurveFloat* newAccelerationCurve) 
{
	spline = newSpline;
	accelerationCurve = newAccelerationCurve;
}

void UComponentFollowSpline::SetSpline(USplineComponent* newSpline, UCurveFloat* newAccelerationCurve, float newDuration, bool newDrawDebug)
{
	spline = newSpline;
	accelerationCurve = newAccelerationCurve;

	if (spline) {
		duration = newDuration;
		spline->Duration = newDuration;
		spline->bDrawDebug = newDrawDebug;
	}
}

void UComponentFollowSpline::AddEvent(float time, FOnTimelineEvent event)
{
	if (timeline) {
		timeline->AddEvent(time, event);
	}
}

void UComponentFollowSpline::OnAccelerationTimelineUpdate(float Alpha)
{
	if (!spline) {
		return;
	}
	
	float progress = FMath::Lerp(0, spline->GetSplineLength(), Alpha);

	AActor* owner = GetOwner();

	FVector position = spline->GetLocationAtDistanceAlongSpline(progress, ESplineCoordinateSpace::World);
	FRotator rotation = spline->GetRotationAtDistanceAlongSpline(progress, ESplineCoordinateSpace::World);
	owner->SetActorLocation(position);
	owner->SetActorRotation(rotation);
}

void UComponentFollowSpline::OnAccelerationTimelineFinished()
{
	UE_LOG(LogTemp, Display, TEXT("Your FINISHED"));
	onSplineFinishedDelegate.ExecuteIfBound();
	// TODO: Should clear timeline events?
	// TODO: Should clear delegates?
}
