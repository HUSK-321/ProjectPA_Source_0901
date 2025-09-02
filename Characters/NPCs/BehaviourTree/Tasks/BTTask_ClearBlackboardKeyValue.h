// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearBlackboardKeyValue.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UBTTask_ClearBlackboardKeyValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	//~Begin UBTTask_BlackboardBase interfaces
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//~End UBTTask_BlackboardBase interfaces
};
