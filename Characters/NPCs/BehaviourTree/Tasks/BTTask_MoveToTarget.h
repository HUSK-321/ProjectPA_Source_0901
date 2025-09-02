// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UBTTask_MoveToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin="0.0"))
	float AcceptableRadius;
	UPROPERTY(EditAnywhere, Category = Node)
	float SpeedMultiplier;
	FVector DestinationLocation;
	UPROPERTY()
	AActor* CachedTargetActor;
	UPROPERTY(Transient)
	TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	UPROPERTY(EditAnywhere)
	bool bRequireNavigableEndLocation;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Node)
	bool bDrawDebugPath;
#endif



public:
	UBTTask_MoveToTarget();
	
	// ~Begin UBTTaskNode Functions
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	// ~End UBTTaskNode Functions
	
private:
	UFUNCTION()
	void OnMoveTaskFinished(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController);
};
