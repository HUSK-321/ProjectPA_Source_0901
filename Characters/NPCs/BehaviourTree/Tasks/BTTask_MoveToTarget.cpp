// All copyrights for this code are owned by Aster.


#include "BTTask_MoveToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Navigation/PathFollowingComponent.h"
#include "Tasks/AITask_MoveTo.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
	: AcceptableRadius(50.0f)
	, SpeedMultiplier(1.0f)
	, DestinationLocation(FVector::ZeroVector)
	, CachedTargetActor(nullptr)
	, bRequireNavigableEndLocation(false)
#if WITH_EDITORONLY_DATA
	, bDrawDebugPath(false)
#endif
{
	NodeName = TEXT("Move To Target");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (IsValid(AIController) == false)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (IsValid(BlackboardComp) == false)
	{
		return EBTNodeResult::Failed;
	}

	if (BlackboardKey.SelectedKeyName.IsValid() == false)
	{
		return EBTNodeResult::Failed;
	}

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		CachedTargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName));
		if (IsValid(CachedTargetActor) == false)
		{
			return EBTNodeResult::Failed;
		}
		DestinationLocation = CachedTargetActor->GetActorLocation();
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		DestinationLocation = BlackboardComp->GetValueAsVector(BlackboardKey.SelectedKeyName);
		CachedTargetActor = nullptr;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	UAITask_MoveTo* MoveTask = UAITask_MoveTo::AIMoveTo(
			AIController,
			DestinationLocation,
			CachedTargetActor,
			AcceptableRadius,
			EAIOptionFlag::Default,
			EAIOptionFlag::Default,
			true,
			true,
			CachedTargetActor != nullptr,
			EAIOptionFlag::Default,
			bRequireNavigableEndLocation ? EAIOptionFlag::Enable : EAIOptionFlag::Disable);

	if (MoveTask)
	{
		MoveTask->OnMoveTaskFinished.AddUObject(this, &UBTTask_MoveToTarget::OnMoveTaskFinished);
		MoveTask->ReadyForActivation();
		CachedOwnerComp = &OwnerComp;
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_MoveToTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	CachedOwnerComp = nullptr;
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_MoveToTarget::OnMoveTaskFinished(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController)
{
	
	if (Result == EPathFollowingResult::Success)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Failed);
	}
}
