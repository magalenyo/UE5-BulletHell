// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComponentFollowSpline.generated.h"

class USplineComponent;
class UTimelineComponent;
class FOnTimelineEvent;

DECLARE_DELEGATE(FOnSplineFinishedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BULLETHELL_API UComponentFollowSpline : public UActorComponent
{
	GENERATED_BODY()

public:	
	UComponentFollowSpline();

protected:
	virtual void BeginPlay() override;

public:	

	FOnSplineFinishedSignature onSplineFinishedDelegate;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartSpline();
	void SetSpline(USplineComponent* newSpline, UCurveFloat* newAccelerationCurve);
	void SetSpline(USplineComponent* newSpline, UCurveFloat* newAccelerationCurve, float newDuration, bool newDrawDebug = false);
	void AddEvent(float time, FOnTimelineEvent event);

private:
	UCurveFloat* accelerationCurve;
	USplineComponent* spline;
	UTimelineComponent* timeline;
	float duration = 0.0f;

	UFUNCTION()
	void OnAccelerationTimelineUpdate(float Alpha);

	UFUNCTION()
	void OnAccelerationTimelineFinished();
};
