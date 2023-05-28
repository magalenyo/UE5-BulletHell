#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomLocationRadius.generated.h"

UCLASS()
class BULLETHELL_API UBTTask_FindRandomLocationRadius : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FindRandomLocationRadius();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	float radius = 3300.0f;

	UPROPERTY(EditAnywhere)
	FVector offset = FVector::ZeroVector;
};
