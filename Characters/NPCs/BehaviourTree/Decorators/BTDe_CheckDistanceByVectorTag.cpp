// All copyrights for this code are owned by Aster.


#include "BTDe_CheckDistanceByVectorTag.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDe_CheckDistanceByVectorTag::UBTDe_CheckDistanceByVectorTag()
{
	NodeName = TEXT("Check Target In Distance - Vector");
}

bool UBTDe_CheckDistanceByVectorTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr)
	{
		return false;
	}
	
	const FVector StartPosition= OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	const FVector TargetPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetVectorKey);
	
	const float Distance = FVector::DistSquared2D(StartPosition, TargetPosition);
	const float DistanceSquared = TargetDistance * TargetDistance;

	const bool bResult = (Distance <= DistanceSquared);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(OutputBoolKey, bResult);
	return bResult;
}
