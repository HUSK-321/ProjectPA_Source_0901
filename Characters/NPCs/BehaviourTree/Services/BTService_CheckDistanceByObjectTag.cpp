#include "BTService_CheckDistanceByObjectTag.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckDistanceByObjectTag::UBTService_CheckDistanceByObjectTag()
{
}

void UBTService_CheckDistanceByObjectTag::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr)
	{
		return;
	}
	
	const FVector StartPosition= OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetObjectKey));
	if (TargetActor == nullptr)
	{
		return;
	}

	const FVector TargetPosition = TargetActor->GetActorLocation();
	const float Distance = FVector::DistSquared2D(StartPosition, TargetPosition);
	const float DistanceSquared = TargetDistance * TargetDistance;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(OutputBoolKey, (Distance <= DistanceSquared));
}
