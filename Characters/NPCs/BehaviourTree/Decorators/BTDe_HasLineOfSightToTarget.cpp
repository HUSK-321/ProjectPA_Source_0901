// All copyrights for this code are owned by Aster.

#include "BTDe_HasLineOfSightToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UBTDe_HasLineOfSightToTarget::UBTDe_HasLineOfSightToTarget()
{
	NodeName = TEXT("Has Line Of Sight To Target");
	TargetActorKeyName = TEXT("TargetActor"); 
}

bool UBTDe_HasLineOfSightToTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (IsValid(AIController) == false)
	{
		return false;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (IsValid(AIPawn) == false)
	{
		return false;
	}
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (IsValid(BlackboardComp) == false)
	{
		return false;
	}

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKeyName));
	if (IsValid(TargetActor) == false)
	{
		return false;
	}

	FVector AIPawnLocation = AIPawn->GetActorLocation();
	FVector TargetActorLocation = TargetActor->GetActorLocation();

	return !PerformLineTrace(AIPawnLocation, TargetActorLocation, AIPawn, TargetActor);
}

bool UBTDe_HasLineOfSightToTarget::PerformLineTrace(const FVector& StartLoc, const FVector& EndLoc, const AActor* InstigatorActor, const AActor* TargetActor) const
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		return false;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorActor);
	Params.AddIgnoredActor(TargetActor);
	
	bool bHit = World->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, Params);

#if WITH_EDITORONLY_DATA
	if (bDrawDebug)
	{
		FColor DrawColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(World, StartLoc, EndLoc, DrawColor, false, 0.5f, 0, 1.0f);
		if (bHit)
		{
			DrawDebugPoint(World, HitResult.ImpactPoint, 10.0f, DrawColor, false, 5.0f);
		}	
	}
#endif
	
	if (bHit && IsValid(HitResult.GetActor()))
	{
		return true;
	}

	return false;
}
