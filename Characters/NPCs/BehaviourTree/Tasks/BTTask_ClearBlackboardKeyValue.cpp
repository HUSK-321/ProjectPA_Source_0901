// All copyrights for this code are owned by Aster.


#include "BTTask_ClearBlackboardKeyValue.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ClearBlackboardKeyValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    
	if (IsValid(BlackboardComp) == false)
	{
		return EBTNodeResult::Failed;
	}

	BlackboardComp->ClearValue(GetSelectedBlackboardKey());
    
	return EBTNodeResult::Succeeded;
}
