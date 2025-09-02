#include "BTDe_CheckDistanceByObjectTag.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDe_CheckDistanceByObjectTag::UBTDe_CheckDistanceByObjectTag()
{
	NodeName = TEXT("Check Target In Distance - Actor");
}

bool UBTDe_CheckDistanceByObjectTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr)
	{
		return false;
	}
	
	const FVector StartPosition= OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetObjectKey));
	if (TargetActor == nullptr)
	{
		return false;
	}

	const FVector TargetPosition = TargetActor->GetActorLocation();
	const float Distance = FVector::DistSquared2D(StartPosition, TargetPosition);
	const float DistanceSquared = TargetDistance * TargetDistance;

	const bool bResult = (Distance <= DistanceSquared);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(OutputBoolKey, bResult);
	return bResult;
}
