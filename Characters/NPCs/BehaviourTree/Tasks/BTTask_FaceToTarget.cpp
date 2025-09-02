#include "BTTask_FaceToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FaceToTarget::UBTTask_FaceToTarget()
{
	NodeName = TEXT("Face To Target");
}

EBTNodeResult::Type UBTTask_FaceToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(OwnerComp.GetAIOwner() == nullptr && OwnerComp.GetBlackboardComponent())
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetPlayerKey));
	if (IsValid(TargetActor) == false)
	{
		return EBTNodeResult::Failed;
	}
	
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorLocation(), TargetActor->GetActorLocation());
	NewRotation.Pitch = 0.f;
	ControllingPawn->SetActorRotation(NewRotation);

	return EBTNodeResult::Succeeded;
}
