// All copyrights for this code are owned by Aster.


#include "BTService_CheckDistanceByVectorTag.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_CheckDistanceByVectorTag::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                   float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (IsValid(OwnerComp.GetAIOwner()->GetPawn()) == false || IsValid(OwnerComp.GetBlackboardComponent()) == false)
	{
		return;
	}
	
	const FVector StartPosition= OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	const FVector TargetPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetVectorKey);
	const float Distance = FVector::DistSquared2D(StartPosition, TargetPosition);
	const float DistanceSquared = TargetDistance * TargetDistance;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(OutputBoolKey, (Distance <= DistanceSquared));
}
