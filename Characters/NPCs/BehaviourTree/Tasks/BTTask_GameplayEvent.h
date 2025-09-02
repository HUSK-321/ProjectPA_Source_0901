// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UBTTask_GameplayEvent : public UBTTaskNode
{
	GENERATED_BODY()

public:
	//~Begin UBTTaskNode Functions
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//~End UBTTaskNode Functions
	
private:
	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayEventTag;
};
